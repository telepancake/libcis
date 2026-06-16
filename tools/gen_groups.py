#!/usr/bin/env python3
"""Consolidated-test stage of the build graph, generated from
test/std/manifest.json.

  --ninja            emit build/groups.ninja: per source directory one group
                     TU edge (deps: the member rec.jsons + manifest + this
                     tool), then per backend compile/link + run edges with
                     real .result outputs, and a board edge summarizing them.
  --emit KEY OUT     write ONE group TU (write-if-changed; the edge is
                     restat=1 so an unchanged TU prunes recompiles).

One TU per directory: the std headers are parsed once per ~7 tests instead of
per test (measured ~7-11x).  transfer.py already isolated each file in its own
namespace, so bodies concatenate verbatim; #includes are hoisted and deduped;
the synthesized main calls each run-test's recorded qualified entry.

All backends compile at gnu++20: the tests were adapted under TEST_STD_VER=20,
so a vendor run at a newer std would activate >=23 arms the transfer never saw
and report failures that say nothing about libcis.
"""
import hashlib
import json
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.chdir(ROOT)
sys.path.insert(0, os.path.join(ROOT, "tools"))
import config as cfg  # noqa: E402

MANIFEST = "test/std/manifest.json"
SRC_DIR = "build/groups/src"
RUN_TIMEOUT = 120  # seconds per test binary

BACKENDS = {
    "libcis":    (cfg.CXX_LIBCIS,    "-std=gnu++20 -fcoroutines -fno-exceptions -fno-rtti -nostdinc++ -Iinclude -Itest/std -Itest/std/support -O0 -w", True),
    "libcxx":    (cfg.CXX_LIBCXX,    "-std=gnu++20 -stdlib=libc++ -fno-exceptions -fno-rtti -Itest/std -Itest/std/support -O0 -w", False),
    "libstdcxx": (cfg.CXX_LIBSTDCXX, "-std=gnu++20 -fno-exceptions -fno-rtti -Itest/std -Itest/std/support -O0 -w", False),
    # discriminator: gcc-10 with its OWN libstdc++.  A test red here and red on
    # libcis is a gcc-10 limitation; red only on libcis is a libcis bug.
    "gcc10std":  (cfg.CXX_GCC10STD,  "-std=gnu++20 -fcoroutines -fno-exceptions -fno-rtti -Itest/std -Itest/std/support -O0 -w", False),
}


def backend_flags(fl, be):
    """ADDITIONAL_COMPILE_FLAGS are written for clang; translate per backend."""
    out = []
    for f in fl:
        if be != "libcxx":
            m = re.match(r"-fconstexpr-steps=(\d+)", f)
            if m:
                out.append(f"-fconstexpr-ops-limit={m.group(1)}")
                continue
            if f.startswith(("-Wno-", "-fsized-deallocation", "-faligned-allocation")):
                continue  # clang spellings g++-10 may reject; tests run with -w anyway
        out.append(f)
    return out
LINK_CIS = "-nodefaultlibs -lpthread -lm -lc -lgcc_s -lgcc"


def gkey(d):
    # injective: sibling dirs op++ / op= / op* collide under plain sanitizing
    return re.sub(r"[^0-9A-Za-z]+", "_", d) + "_" + hashlib.sha1(d.encode()).hexdigest()[:8]


# Adaptive split thresholds (see load_groups).  Whole-feature consolidation is
# right for most features (~10-30 tests, <100 KiB collated source); the same TU
# for utilities/tuple instantiates a peak of 14.8 GB RSS in cc1plus on g++-13.
# When a feature trips either threshold, we drop one level deeper (sub-feature
# per TU) just for that feature.  Some sub-features bottom out before becoming
# small enough -- utilities/tuple/tuple.tuple/tuple.cnstr is 28 files / 86 KiB
# in one flat directory -- and CHUNK_TESTS then partitions them into N-test
# slices so cc1plus never has to instantiate the whole pile in one TU.
SPLIT_TEST_COUNT = 25
SPLIT_SRC_BYTES  = 80 * 1024
CHUNK_TESTS      = 5


def _feature_key(path):
    """Top-2 path components, e.g. utilities/tuple."""
    return "/".join(os.path.dirname(path).split("/")[:2])


def _key_at_depth(path, depth):
    """First `depth` components of the file's directory, or all if fewer."""
    parts = os.path.dirname(path).split("/")
    return "/".join(parts[: max(depth, 1)])


def _src_bytes(members):
    """Sum of test source bytes for a group's members; 0 on read error."""
    n = 0
    for r in members:
        try:
            n += os.path.getsize(os.path.join("test/std", r["file"]))
        except OSError:
            pass
    return n


def load_groups():
    man = json.load(open(MANIFEST))
    # exclusions.json: tests removed from the binary CLEAN/NOT-CLEAN verdict
    # (each with a written justification). Honor it here too — including these
    # in a group's TU would bring back the compiler limitation the exclusion
    # is documenting (e.g. recursion_depth.pass.cpp OOMs cc1plus on both
    # g++-10 and g++-13).
    try:
        excl = {k for k, v in json.load(open("tools/exclusions.json")).items()
                if not k.startswith("_")}
    except FileNotFoundError:
        excl = set()
    seen, rows, skipped = set(), [], 0
    for r in man["transferred"]:
        if r["file"] in seen:
            continue
        seen.add(r["file"])
        if r["file"] in excl:
            skipped += 1
            continue
        rows.append(r)
    if skipped:
        print(f"  ({skipped} tests excluded per tools/exclusions.json)",
              file=sys.stderr)

    # Start at depth 2 (feature: utilities/optional, containers/sequences).
    # If a group trips either threshold, deepen JUST THAT GROUP by one level
    # until the threshold is satisfied or the directory bottoms out.  This
    # keeps tuple split deeply (utilities/tuple -> tuple.tuple -> tuple.rel/
    # tuple.apply/...) while leaving e.g. utilities/optional whole.
    pending = {}  # key -> (depth, members)
    for r in rows:
        k = _key_at_depth(r["file"], 2)
        pending.setdefault(k, [2, []])[1].append(r)
    # Rewrite shape to (k -> [depth, members])
    work = list(pending.items())

    groups = {}
    while work:
        k, (depth, members) = work.pop()
        too_big = (len(members) > SPLIT_TEST_COUNT or
                   _src_bytes(members) > SPLIT_SRC_BYTES)
        # See if a deeper split is even possible: at least two distinct
        # next-level subdirs (otherwise deepening just renames the group).
        next_keys = {_key_at_depth(r["file"], depth + 1) for r in members}
        if too_big and len(next_keys) >= 2:
            # Regroup members by next depth and re-enqueue.
            by_sub = {}
            for r in members:
                by_sub.setdefault(_key_at_depth(r["file"], depth + 1), []).append(r)
            for nk, sub in by_sub.items():
                work.append((nk, [depth + 1, sub]))
        elif too_big:
            # Flat-bottomed: can't deepen further (one subdir or already at
            # a leaf).  Slice into CHUNK_TESTS-sized windows over the
            # filename-sorted members.  The sort is what makes the chunks
            # deterministic across re-runs; chunk keys append `_chunk_<i>`
            # to the unsplittable directory key so they're stable too.
            members_sorted = sorted(members, key=lambda r: r["file"])
            for i in range(0, len(members_sorted), CHUNK_TESTS):
                groups[f"{k}_chunk_{i // CHUNK_TESTS}"] = members_sorted[i:i + CHUNK_TESTS]
        else:
            groups[k] = members
    return groups


def resolve_quote_include(line, filedir):
    """Hoisting moves #include lines away from the file whose directory
    anchors them; re-anchor relative quote-includes ("../types.h", "types.h")
    to a test/std-rooted path so they survive consolidation."""
    m = re.match(r'(\s*#\s*include\s*)"([^"]+)"(.*)$', line, re.S)
    if not m:
        return line
    rel = os.path.normpath(os.path.join("test/std", filedir, m.group(2)))
    if os.path.exists(rel):
        return f'{m.group(1)}"{os.path.relpath(rel, "test/std")}"{m.group(3)}'
    return line


def group_source(members, key):
    inc, seen, bodies, calls = [], set(), [], []
    for r in sorted(members, key=lambda r: r["file"]):
        filedir = os.path.dirname(r["file"])
        body = []
        for ln in open(os.path.join("test/std", r["file"]), errors="replace").read().splitlines(True):
            if re.match(r"\s*#\s*include", ln):
                ln = resolve_quote_include(ln, filedir)
                if ln.strip() not in seen:
                    seen.add(ln.strip())
                    inc.append(ln if ln.endswith("\n") else ln + "\n")
            else:
                body.append(ln)
        bodies.append("".join(body) + "\n")
        if r["kind"] == "run" and r.get("entry"):
            # fork-per-test: each test runs in its own child, so an abort/
            # crash (assert, segfault, throw-under-fno-exceptions) is contained
            # and attributed instead of blanking the whole consolidated group,
            # and per-test global state cannot leak between tests.
            calls.append(f'  libcis_run([]() -> int {{ return {r["entry"]}; }}, "{r["slug"]}");')
    # The harness is TU-local (`static`/anonymous-namespace-equivalent via the
    # `static` keyword + a TU-private template) so 200 group TUs linked into
    # one ELF do not collide on these symbols.  The per-group RUN function is
    # named after the group key (extern "C" so the driver can forward-declare
    # without name-mangling guesswork), and writes its totals out by reference.
    # The wrapper `main` calls that same function -- it's only enabled when
    # LIBCIS_SINGLE_ELF is NOT defined, so the per-group `.exe` and the
    # single-ELF `.o` come from the same source via different -D flags.
    harness = (
        "#include <cstdio>\n#include <unistd.h>\n#include <sys/wait.h>\n"
        "namespace { int libcis_total = 0, libcis_fails = 0; }\n"
        "template<class F> static void libcis_run(F f, const char* name) {\n"
        "  ++libcis_total;\n"
        "  ::pid_t p = ::fork();\n"
        "  if (p == 0) { ::alarm(30); ::_exit(f()); }  // per-test wall clock + own exit code\n"
        "  int st = 0; ::waitpid(p, &st, 0);\n"
        "  bool ok = WIFEXITED(st) && WEXITSTATUS(st) == 0;\n"
        "  if (!ok) { ++libcis_fails; ::fprintf(stderr, \"LIBCIS-FAIL %s (status=%d)\\n\", name, st); }\n"
        "}\n")
    group_fn = (
        f'extern "C" void libcis_run_group_{key}(int& out_total, int& out_fails) {{\n'
        + "\n".join(calls)
        + "\n  out_total += libcis_total;\n"
        + "  out_fails += libcis_fails;\n"
        + "}\n")
    # the trailing LIBCIS-DONE line lets the board count per-TEST and detect
    # groups that crashed/hung before finishing (no DONE line == incomplete).
    wrapper = (
        "#ifndef LIBCIS_SINGLE_ELF\n"
        "int main(){\n"
        "  int t = 0, f = 0;\n"
        f"  libcis_run_group_{key}(t, f);\n"
        '  ::fprintf(stderr, "LIBCIS-DONE total=%d fails=%d\\n", t, f);\n'
        "  return f ? 1 : 0;\n"
        "}\n"
        "#endif\n")
    return ("".join(inc) + "\n" + harness + "".join(bodies)
            + "\n" + group_fn + "\n" + wrapper)


def write_if_changed(path, text):
    if os.path.exists(path) and open(path, errors="replace").read() == text:
        return False
    os.makedirs(os.path.dirname(path), exist_ok=True)
    open(path, "w").write(text)
    return True


def emit_one(key, out):
    for d, members in load_groups().items():
        if gkey(d) == key:
            write_if_changed(out, group_source(members, key))
            return
    sys.exit(f"no group {key} in {MANIFEST}")


def emit_driver(out, keys):
    """The single-ELF driver: forward-declare every group's run function and
    call them in sorted key order, accumulating totals.  Same LIBCIS-DONE
    summary the per-group main prints, so the existing log scrapers (board.py
    via LIBCIS-FAIL <slug>) keep working on the consolidated run too."""
    decls = "\n".join(f'extern "C" void libcis_run_group_{k}(int&, int&);' for k in keys)
    calls = "\n".join(f"  libcis_run_group_{k}(t, f);" for k in keys)
    src = (
        "#include <cstdio>\n"
        + decls + "\n"
        "int main() {\n"
        "  int t = 0, f = 0;\n"
        + calls + "\n"
        '  ::fprintf(stderr, "LIBCIS-DONE total=%d fails=%d\\n", t, f);\n'
        "  return f ? 1 : 0;\n"
        "}\n")
    write_if_changed(out, src)


def emit_ninja():
    groups = load_groups()
    flags = {d: sorted(set(f for r in ms for f in r.get("flags", [])))
             for d, ms in groups.items()}
    L = ["ninja_required_version = 1.10",
         "rule gengroup",
         "  command = python3 tools/gen_groups.py --emit $key $out",
         "  description = GROUP $out",
         "  restat = 1",
         "rule gendriver",
         "  command = python3 tools/gen_groups.py --driver $out",
         "  description = DRIVER $out",
         "  restat = 1",
         "rule run",
         f'  command = ( timeout {RUN_TIMEOUT} "$in" >$out 2>&1; echo exit=$$? >>$out )',
         "  description = RUN $in",
         ""]
    for be, (cxx, cargs, has_sup) in BACKENDS.items():
        L += [f"cxx_{be} = {cxx}", f"cargs_{be} = {cargs}",
              f"rule link_{be}",
              f"  command = $cxx_{be} $cargs_{be} $flags $in $extra_{be} -o $out -MMD -MF $out.d",
              "  depfile = $out.d", "  deps = gcc",
              f"  description = LINK[{be}] $out", ""]
    # support.cpp provides operator new/delete (mandatory under -nodefaultlibs).
    # It must be linked as an ARCHIVE, not a bare .o: tests using libc++'s
    # count_new.h define their own operator new/delete in the group TU, and
    # replaceable-function semantics require the runtime's definitions to be
    # archive members the linker only pulls when nothing else defines them
    # (exactly how libstdc++.a ships operator new).  A bare .o made every
    # count_new group a duplicate-definition link error.
    sup = "build/groups/libcis/__support.o"
    suplib = "build/groups/libcis/libsupport.a"
    # Single-ELF mode: per-group .cpp is recompiled to .single.o with
    # -DLIBCIS_SINGLE_ELF so the wrapper main() drops out, leaving only
    # libcis_run_group_<key>().  All .single.o files + the driver TU link
    # into one all_tests.exe.  The per-group .o and .single.o are DIFFERENT
    # objects from the same .cpp source -- no symbol clashes possible.
    L += ["rule cco",
          "  command = $cxx_libcis $cargs_libcis -c $in -o $out -MMD -MF $out.d",
          "  depfile = $out.d", "  deps = gcc",
          "rule cco_single",
          "  command = $cxx_libcis $cargs_libcis -DLIBCIS_SINGLE_ELF $flags -c $in -o $out -MMD -MF $out.d",
          "  depfile = $out.d", "  deps = gcc",
          "  description = CXX[single] $out",
          "rule link_single",
          f"  command = $cxx_libcis $cargs_libcis $in {suplib} {LINK_CIS} -o $out",
          "  description = LINK[single] $out",
          "rule ar",
          "  command = rm -f $out && ar rcs $out $in",
          "  description = AR $out",
          f"build {sup}: cco src/support.cpp",
          f"build {suplib}: ar {sup}",
          f"extra_libcis = {suplib} {LINK_CIS}",
          "extra_libcxx =", "extra_libstdcxx = ", "extra_gcc10std =", ""]

    results = {be: [] for be in BACKENDS}
    single_objs = []
    keys_sorted = []
    for d in sorted(groups):
        key = gkey(d)
        keys_sorted.append(key)
        src = f"{SRC_DIR}/{key}.cpp"
        recs = " ".join(f"build/recs/{r['file']}.rec.json" for r in groups[d])
        L += [f"build {src}: gengroup {recs} | tools/gen_groups.py {MANIFEST}",
              f"  key = {key}"]
        # Per-group flow (existing): one edge per backend, compile+link in one.
        for be in BACKENDS:
            dep = f" | {suplib}" if be == "libcis" else ""
            exe, res = f"build/groups/{be}/{key}.exe", f"build/groups/{be}/{key}.result"
            L.append(f"build {exe}: link_{be} {src}{dep}")
            fl = " ".join(backend_flags(flags[d], be))
            if fl:
                L.append(f"  flags = {fl}")
            L.append(f"build {res}: run {exe}")
            results[be].append(res)
        # Single-ELF flow (libcis only): compile each group .cpp once more with
        # -DLIBCIS_SINGLE_ELF, drop the wrapper main, keep libcis_run_group_*.
        sobj = f"build/groups/libcis/{key}.single.o"
        L.append(f"build {sobj}: cco_single {src}")
        fl = " ".join(backend_flags(flags[d], "libcis"))
        if fl:
            L.append(f"  flags = {fl}")
        single_objs.append(sobj)

    # The driver TU: forward-declares libcis_run_group_<key> for every key
    # and provides the single main() that calls them in sorted-key order.
    driver_src = "build/groups/libcis/__alltests.cpp"
    driver_obj = "build/groups/libcis/__alltests.o"
    single_exe = "build/groups/libcis/all_tests.exe"
    single_res = "build/groups/libcis/all_tests.result"
    L += [f"build {driver_src}: gendriver | tools/gen_groups.py {MANIFEST}",
          f"build {driver_obj}: cco {driver_src}",
          f"build {single_exe}: link_single {' '.join(single_objs)} {driver_obj} | {suplib}",
          f"build {single_res}: run {single_exe}",
          f"build single: phony {single_res}",
          ""]

    # a red group = failed link edge = missing .result; run with -k0 and let
    # tools/board.py count the holes -- no edge may swallow a failure.
    for be in BACKENDS:
        L.append(f"build {be}: phony " + " $\n    ".join(results[be]))
    L += ["default libcis", ""]
    write_if_changed("build/groups.ninja", "\n".join(L))
    print(f"build/groups.ninja: {len(groups)} groups x {len(BACKENDS)} backends "
          f"(+ single-ELF: {len(single_objs)} .single.o + 1 driver)")


if __name__ == "__main__":
    if len(sys.argv) >= 2 and sys.argv[1] == "--ninja":
        emit_ninja()
    elif len(sys.argv) >= 4 and sys.argv[1] == "--emit":
        emit_one(sys.argv[2], sys.argv[3])
    elif len(sys.argv) >= 3 and sys.argv[1] == "--driver":
        emit_driver(sys.argv[2], sorted(gkey(d) for d in load_groups()))
    else:
        sys.exit("usage: gen_groups.py --ninja | --emit KEY OUT | --driver OUT")
