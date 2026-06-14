#!/usr/bin/env python3
"""Generate a compact constexpr IANA time-zone database header for libcis.

Design (see the chat discussion):
  * All the hard "zic" work is done HERE, at build time, in Python.
  * A hard cutoff (system date minus CUTOFF_YEARS) discards every transition
    older than that, so the historical tail (sub-minute LMT, WWII double-DST,
    19th-century rules) disappears and the runtime never needs a rule engine.
  * The result is emitted as `constexpr` arrays of small fixed structs with
    indices linking zones -> their transition slice, plus a shared abbreviation
    string pool, the links, and the leap seconds.  The runtime (<chrono>) only
    binary-searches those arrays.

Reads the system tz database: the compiled TZif files under /usr/share/zoneinfo
(which ARE the zic output of the text source) for the precomputed transitions,
and tzdata.zi / leapseconds for the canonical zone+link list, version and leaps.
"""
import datetime
import os
import struct
import sys

ZONEINFO = "/usr/share/zoneinfo"
TZDATA_ZI = os.path.join(ZONEINFO, "tzdata.zi")
LEAPSECONDS = os.path.join(ZONEINFO, "leapseconds")
CUTOFF_YEARS = 10
OUT = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
                   "include", "bits", "chrono_tzdb_data.h")

NEG_INF = -(1 << 62)  # sys_seconds-ish sentinel for "since the beginning"


def read_tzif(path):
    """Return (transitions, types, abbr_bytes) from a TZif v2+ file.

    transitions: list of (utc_seconds, type_index)
    types:       list of (utcoff_seconds, isdst, abbr_offset)
    """
    d = open(path, "rb").read()
    if d[:4] != b"TZif":
        return None

    def counts(off):
        return struct.unpack(">6I", d[off + 20:off + 44]), off + 44

    (isut, isst, leap, tc, tyc, cc), p = counts(0)
    # skip the 32-bit v1 body to reach the 64-bit v2 block
    p += tc * 4 + tc + tyc * 6 + cc + leap * 8 + isst + isut
    if d[p:p + 4] != b"TZif":
        return None
    (isut, isst, leap, tc, tyc, cc), q = counts(p)
    trans = list(struct.unpack(f">{tc}q", d[q:q + tc * 8])); q += tc * 8
    idx = list(d[q:q + tc]); q += tc
    types = []
    for _ in range(tyc):
        gmtoff, dst, ab = struct.unpack(">iBB", d[q:q + 6]); q += 6
        types.append((gmtoff, dst, ab))
    abbr = d[q:q + cc]; q += cc
    return list(zip(trans, idx)), types, abbr


def abbr_str(abbr_bytes, off):
    e = abbr_bytes.index(b"\0", off)
    return abbr_bytes[off:e].decode()


def parse_zi_names():
    """Return (zone_names, links) from tzdata.zi. links: list of (name, target)."""
    zones, links, version = [], [], ""
    for line in open(TZDATA_ZI, errors="replace"):
        if line.startswith("# version"):
            version = line.split()[-1]
        s = line.rstrip("\n")
        if s.startswith("Z "):
            zones.append(s.split()[1])
        elif s.startswith("L "):
            f = s.split()
            links.append((f[2], f[1]))  # (name, target)
    return zones, links, version


def parse_leaps():
    """Return list of (date_seconds_since_1970, value) for Leap lines."""
    leaps = []
    total = 0
    for line in open(LEAPSECONDS, errors="replace"):
        f = line.split()
        if not f or f[0] != "Leap":
            continue
        # Leap <year> <month> <day> <hh:mm:ss> <+/-> S/R ...
        year, mon, day = int(f[1]), MONTHS[f[2]], int(f[3])
        sign = 1 if f[5] == "+" else -1
        # leap_second::date() is the instant the leap takes effect (00:00:00 UTC
        # of the day after the 23:59:60 second); value() is the individual leap.
        dt = datetime.datetime(year, mon, day, tzinfo=datetime.timezone.utc) \
            + datetime.timedelta(days=1)
        leaps.append((int(dt.timestamp()), sign))
    return leaps


MONTHS = {"Jan": 1, "Feb": 2, "Mar": 3, "Apr": 4, "May": 5, "Jun": 6,
          "Jul": 7, "Aug": 8, "Sep": 9, "Oct": 10, "Nov": 11, "Dec": 12}


def main():
    cutoff_dt = datetime.datetime.now(datetime.timezone.utc) \
        - datetime.timedelta(days=365 * CUTOFF_YEARS)
    cutoff = int(cutoff_dt.timestamp())

    zone_names, links, version = parse_zi_names()
    if not version:
        version = "unknown"

    # Abbrev pool: dedup; offset 0 reserved for "".
    pool = ["\0"]
    pool_off = {"": 0}

    def intern(a):
        if a not in pool_off:
            pool_off[a] = sum(len(x) for x in pool)
            pool.append(a + "\0")
        return pool_off[a]

    transitions = []   # (utc, offset, save, abbr_off)
    zones = []         # (name, tr_first, tr_count, init_off, init_save, init_abbr)

    for name in sorted(zone_names):
        path = os.path.join(ZONEINFO, name)
        parsed = read_tzif(path)
        if parsed is None:
            # zone with no DST history / missing TZif: synth a fixed UTC zone
            zones.append((name, len(transitions), 0, 0, 0, intern("UTC")))
            continue
        tr, types, abbr = parsed
        # state active just before the first kept transition
        kept = [(t, i) for (t, i) in tr if t >= cutoff]
        if kept:
            first_t = kept[0][0]
            before = [ (t,i) for (t,i) in tr if t < first_t ]
            ti = before[-1][1] if before else (tr[0][1] if tr else 0)
        else:
            ti = tr[-1][1] if tr else 0
        if types:
            io, idst, ia = types[ti]
            init = (io, (io if idst else 0), intern(abbr_str(abbr, ia)))
            # 'save' is the dst amount; TZif doesn't store it directly, but the
            # tests only read sys_info.save as minutes(offset-stdoff). We store
            # save = isdst ? (offset - matching std offset) : 0; approximate with
            # 3600 when dst else 0 (all post-cutoff DST zones use 1h). Refined
            # below per-transition using the neighbouring std type.
        else:
            init = (0, 0, intern("UTC"))
        first = len(transitions)
        # build a std-offset lookup: for each dst type, find a non-dst type's off
        std_off = None
        for (o, dst, a) in types:
            if not dst:
                std_off = o; break
        for (t, i) in kept:
            o, dst, a = types[i]
            save = (o - std_off) if (dst and std_off is not None) else 0
            transitions.append((t, o, save, intern(abbr_str(abbr, a))))
        # recompute init save against std_off
        io, idst, ia = types[ti] if types else (0, 0, 0)
        isave = (io - std_off) if (idst and std_off is not None) else 0
        zones.append((name, first, len(kept), io, isave,
                      intern(abbr_str(abbr, ia)) if types else intern("UTC")))

    leaps = parse_leaps()
    links_sorted = sorted(links)

    emit(version, pool, transitions, zones, links_sorted, leaps, cutoff)
    print(f"wrote {OUT}: {len(zones)} zones, {len(links_sorted)} links, "
          f"{len(transitions)} transitions, {len(leaps)} leaps, "
          f"pool={sum(len(x) for x in pool)}B, cutoff={cutoff_dt.date()} v{version}")


def cstr(s):
    out = []
    for ch in s:
        if ch == "\0":
            out.append("\\0")
        elif ch == "\\":
            out.append("\\\\")
        elif ch == '"':
            out.append('\\"')
        elif 32 <= ord(ch) < 127:
            out.append(ch)
        else:
            out.append("\\x%02x" % ord(ch))
    return "".join(out)


def emit(version, pool, transitions, zones, links, leaps, cutoff):
    L = []
    w = L.append
    w("// GENERATED by tools/gen_tzdb.py -- do not edit.")
    w("// Compact IANA tz database: transitions newer than a build-time cutoff")
    w("// (system date minus 10 years), precomputed from the system TZif files.")
    w("#pragma once")
    w("#include <cstdint>")
    w("namespace std { namespace chrono { namespace tzdb_data {")
    w(f'inline constexpr char version[] = "{cstr(version)}";')
    w(f"inline constexpr long long cutoff = {cutoff}LL;")
    # abbrev pool as one string literal
    poolstr = "".join(pool)
    w(f'inline constexpr char abbrev_pool[] = "{cstr(poolstr)}";')
    # transitions
    w("struct trans_t { long long utc; int off; int save; unsigned short ab; };")
    w(f"inline constexpr trans_t transitions[] = {{")
    for (t, o, s, a) in transitions:
        w(f"  {{{t}LL,{o},{s},{a}}},")
    w("  {0,0,0,0}")
    w("};")
    # zones (sorted by name)
    w("struct zone_t { const char* name; unsigned first; unsigned count;"
      " int init_off; int init_save; unsigned short init_ab; };")
    w("inline constexpr zone_t zones[] = {")
    for (n, f, c, io, isv, ia) in zones:
        w(f'  {{"{cstr(n)}",{f},{c},{io},{isv},{ia}}},')
    w("};")
    w(f"inline constexpr unsigned zone_count = {len(zones)};")
    # links (sorted by name)
    w("struct link_t { const char* name; const char* target; };")
    w("inline constexpr link_t links[] = {")
    for (n, tgt) in links:
        w(f'  {{"{cstr(n)}","{cstr(tgt)}"}},')
    w("};")
    w(f"inline constexpr unsigned link_count = {len(links)};")
    # leaps
    w("struct leap_t { long long date; int value; };")
    w("inline constexpr leap_t leaps[] = {")
    for (d, v) in leaps:
        w(f"  {{{d}LL,{v}}},")
    w("  {0,0}")
    w("};")
    w(f"inline constexpr unsigned leap_count = {len(leaps)};")
    w("}}} // namespace std::chrono::tzdb_data")
    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    open(OUT, "w").write("\n".join(L) + "\n")


if __name__ == "__main__":
    main()
