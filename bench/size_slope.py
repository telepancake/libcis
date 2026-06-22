#!/usr/bin/env python3
"""size_slope.py — the libcis size/memory/codegen harness.  Run with NO arguments:

        python3 bench/size_slope.py        (or: make size)

It builds everything itself and writes a results tree to .test_results/latest/.
The terminal prints only a short summary and then NAMES the files to open — the
detail (per-call disassembly, CSVs) lives on disk on purpose, so it survives a
`| tail` and a change's real effect on generated code can't be skipped.

WHAT IT MEASURES, and against what
----------------------------------
Everything is reported as OVERHEAD vs a FIXED non-type-erased reference — the
host's libstdc++ — rebuilt every run.  That anchor never drifts, so numbers from
different runs (and different library versions) are directly comparable.  No
"vs the previous commit" baseline to move.

  CODE (headline)  per-call overhead.  bench/callsites.cpp puts each container
                   operation in its own noinline function whose entire body IS
                   the code emitted at one call site.  The symbol's size is the
                   per-call code size; its annotated disassembly is saved per
                   architecture.  Extracted for every target toolchain present:
                   x86_64, i586, arm32, arm64, mips32 (others skipped with a hint).

  CODE (demoted)   per-type overhead.  bench/firmware.cpp instantiated over a
                   growing number of distinct element types; the slope is the
                   marginal flash per added instantiation.  Shown small.

  CODE (omitted)   static / one-time shared-core cost is paid once for the whole
                   image, so it is NOT in the summary — written to static.txt only.

  MEMORY           bench/memprobe.cpp over representative workloads, reporting for
                   each: internal (the structure's own byte accounting), heap
                   (real process heap in use, glibc mallinfo2), and peak stack.

The size numbers are toolchain-relative; the reference build makes them
comparable regardless of which g++ runs.  Pin CXX=g++-10 to match the project's
canonical target.
"""
import csv, os, re, shutil, subprocess, sys, time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
INC = os.path.join(ROOT, "include")
SUPPORT = os.path.join(ROOT, "src", "support.cpp")
RESULTS = os.path.join(ROOT, ".test_results", "latest")

BASE_CXXFLAGS = ["-std=gnu++20", "-fno-exceptions", "-fno-rtti", "-Os", "-w", "-g"]
LTO = ["-flto", "-fmerge-all-constants", "-ffunction-sections", "-fdata-sections",
       "-Wl,--gc-sections", "-fvisibility=hidden", "-fvisibility-inlines-hidden",
       "-fno-asynchronous-unwind-tables", "-fno-unwind-tables"]
if shutil.which("ld.gold"):
    LTO += ["-fuse-ld=gold", "-Wl,--icf=safe"]

# Library variants: the compile/link difference is the ONLY thing that changes.
LIBCIS_C = ["-nostdinc++", "-I" + INC]
LIBCIS_L = [SUPPORT, "-nodefaultlibs", "-lpthread", "-lm", "-lc", "-lgcc_s", "-lgcc"]
REF_C = []                                   # host libstdc++ (non-type-erased)
REF_L = ["-lpthread", "-lm"]

CXX = os.environ.get("CXX", "g++-10")
if not shutil.which(CXX):
    CXX = "g++"

# arch -> how to compile an object for it, how to disassemble it, install hint.
TARGETS = [
    ("x86_64", [CXX, "-m64"],                      "objdump",                       None),
    ("i586",   [CXX, "-m32", "-march=i586"],       "objdump",                       "g++-multilib"),
    ("arm64",  ["aarch64-linux-gnu-g++"],          "aarch64-linux-gnu-objdump",     "g++-aarch64-linux-gnu"),
    ("arm32",  ["arm-linux-gnueabihf-g++"],        "arm-linux-gnueabihf-objdump",   "g++-arm-linux-gnueabihf"),
    ("mips32", ["mips-linux-gnu-g++"],             "mips-linux-gnu-objdump",        "g++-mips-linux-gnu"),
]
CALLSITE_OPS = ["cs_vec_push_int", "cs_vec_push_H", "cs_vec_insert_H", "cs_vec_erase_H",
                "cs_vec_assign_H", "cs_vec_sort_H", "cs_str_append", "cs_str_compare",
                # trivial accessors — must stay tiny; an ops-call here is a blowup
                "cs_vec_size_i", "cs_vec_empty_i", "cs_vec_end_i", "cs_vec_index_i"]


def sh(cmd):
    return subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)


def lib_provenance():
    """Identify exactly which library tree this result measured — so a result can
    never be mistaken for a different branch (the way a hand-written 'finding' can)."""
    b = sh(["git", "-C", ROOT, "rev-parse", "--abbrev-ref", "HEAD"]).stdout.strip()
    c = sh(["git", "-C", ROOT, "rev-parse", "--short", "HEAD"]).stdout.strip()
    dirty = "-dirty" if sh(["git", "-C", ROOT, "status", "--porcelain", "--", "include", "src"]).stdout.strip() else ""
    return "%s @ %s%s" % (b or "?", c or "?", dirty)


def fresh_results():
    if os.path.exists(RESULTS):
        shutil.rmtree(RESULTS)
    os.makedirs(RESULTS)


def section_bytes(binary, *names):
    r = sh(["size", "-A", binary])
    total = 0
    for line in r.stdout.splitlines():
        m = re.match(r"^(\.\S+)\s+(\d+)", line)
        if m and m.group(1) in names:
            total += int(m.group(2))
    return total


def fit(xs, ys):
    n = len(xs); sx, sy = sum(xs), sum(ys)
    sxx = sum(x * x for x in xs); sxy = sum(x * y for x, y in zip(xs, ys))
    slope = (n * sxy - sx * sy) / (n * sxx - sx * sx)
    return slope, (sy - slope * sx) / n


# ---------------------------------------------------------------- call sites
def nm_sizes(obj):
    r = sh(["nm", "-S", "-t", "d", "--defined-only", obj])   # native nm reads any-arch ELF symbols
    out = {}
    for line in r.stdout.splitlines():
        p = line.split()
        if len(p) == 4 and p[3] in CALLSITE_OPS:
            out[p[3]] = int(p[1])
    return out


def disasm(objdump, obj, sym, dst, header):
    cmd = [objdump, "-dl", "--disassemble=" + sym, obj]
    r = sh(cmd)
    body = r.stdout if r.returncode == 0 and sym in r.stdout else \
        "; (disassembly unavailable: %s could not disassemble this object)\n" % objdump
    with open(dst, "w") as f:
        f.write(header + body)


def module_callsites(targets, log):
    src = os.path.join(ROOT, "bench", "callsites.cpp")
    rows = []          # (arch, op, libcis_bytes, ref_bytes, overhead)
    for arch, cxx, objdump, _hint in targets:
        objs = {}
        ok = True
        for lib, cflags in (("libcis", LIBCIS_C), ("ref", REF_C)):
            obj = os.path.join(RESULTS, "callsites", arch, lib + ".o")
            os.makedirs(os.path.dirname(obj), exist_ok=True)
            r = sh(cxx + BASE_CXXFLAGS + cflags + ["-c", src, "-o", obj])
            if r.returncode != 0:
                log.append("  callsites/%s/%s: COMPILE FAILED\n%s" % (arch, lib, r.stdout[-1500:]))
                ok = False
                break
            objs[lib] = obj
        if not ok:
            continue
        sz = {lib: nm_sizes(o) for lib, o in objs.items()}
        od = objdump if shutil.which(objdump) else ("objdump" if arch in ("x86_64", "i586") else None)
        for op in CALLSITE_OPS:
            lc, rf = sz["libcis"].get(op), sz["ref"].get(op)
            if lc is None or rf is None:
                continue
            rows.append((arch, op, lc, rf, lc - rf))
            if od:
                for lib in ("libcis", "ref"):
                    dst = os.path.join(RESULTS, "callsites", arch, lib, op + ".asm")
                    os.makedirs(os.path.dirname(dst), exist_ok=True)
                    hdr = ("; call site: %s   lib=%s   arch=%s   size=%d bytes\n"
                           "; (source-annotated; this is the actual code emitted at one use)\n\n"
                           % (op, lib, arch, sz[lib][op]))
                    disasm(od, objs[lib], op, dst, hdr)
    with open(os.path.join(RESULTS, "per_call_overhead.csv"), "w", newline="") as f:
        w = csv.writer(f); w.writerow(["arch", "op", "libcis_bytes", "ref_bytes", "overhead_bytes"])
        w.writerows(rows)
    return rows


# ---------------------------------------------------------------- per type
def module_pertype(log):
    src = os.path.join(ROOT, "bench", "firmware.cpp")
    sweep = [4, 12, 24]
    series = {"libcis": [], "ref": []}
    for nt in sweep:
        for lib, cflags, lflags in (("libcis", LIBCIS_C, LIBCIS_L), ("ref", REF_C, REF_L)):
            out = "/tmp/fw_%s_%d" % (lib, nt)
            r = sh([CXX] + BASE_CXXFLAGS + LTO + cflags + ["-DNT=%d" % nt, src] + lflags + ["-o", out])
            if r.returncode != 0:
                log.append("  per_type %s NT=%d: BUILD FAILED\n%s" % (lib, nt, r.stdout[-1200:]))
                return None
            series[lib].append(section_bytes(out, ".text", ".rodata"))
    sl_lc, ic_lc = fit(sweep, series["libcis"])
    sl_rf, ic_rf = fit(sweep, series["ref"])
    with open(os.path.join(RESULTS, "per_type.csv"), "w", newline="") as f:
        w = csv.writer(f); w.writerow(["NT"] + ["libcis", "ref"])
        for i, nt in enumerate(sweep):
            w.writerow([nt, series["libcis"][i], series["ref"][i]])
    with open(os.path.join(RESULTS, "static.txt"), "w") as f:
        f.write("one-time shared-core flash (y-intercept; paid ONCE per image, "
                "intentionally omitted from the summary)\n")
        f.write("  libcis   %10.0f B\n  ref      %10.0f B\n" % (ic_lc, ic_rf))
    return {"per_type_libcis": sl_lc, "per_type_ref": sl_rf, "per_type_overhead": sl_lc - sl_rf}


# ---------------------------------------------------------------- memory
def module_memory(log):
    src = os.path.join(ROOT, "bench", "memprobe.cpp")
    data = {}
    for lib, cflags, lflags in (("libcis", LIBCIS_C, LIBCIS_L), ("ref", REF_C, REF_L)):
        out = "/tmp/mem_%s" % lib
        r = sh([CXX] + BASE_CXXFLAGS + cflags + [src] + lflags + ["-o", out])
        if r.returncode != 0:
            log.append("  memory %s: BUILD FAILED\n%s" % (lib, r.stdout[-1200:]))
            return None
        run = sh([out])
        for line in run.stdout.splitlines():
            p = line.split()
            if len(p) == 6 and p[0] == "MEM":
                data.setdefault(p[1], {})[lib] = tuple(int(x) for x in p[2:])
    rows = []
    for name, d in data.items():
        if "libcis" in d and "ref" in d:
            n, ic_i, ic_h, ic_s = d["libcis"]
            _, rf_i, rf_h, rf_s = d["ref"]
            rows.append((name, n, ic_i, rf_i, ic_h, rf_h, ic_h - rf_h, ic_s, rf_s))
    with open(os.path.join(RESULTS, "memory.csv"), "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["bench", "n", "internal_libcis", "internal_ref",
                    "heap_libcis", "heap_ref", "heap_overhead", "stack_libcis", "stack_ref"])
        w.writerows(rows)
    return rows


# ---------------------------------------------------------------- speed
# Each workload is named with what it ACTUALLY does (N, element type). A speed
# number without that context is meaningless, and a single per-"method"
# multiplier is worse than meaningless for an algorithm: "sort 4x" hides that the
# cost is the element's string compare/move that sort merely amortizes. So the
# speed axis reports, per workload, the total instructions AND where they go
# (callgrind per-function attribution) — never a bare multiplier.
SPEED_WL = {
    "cs_vec_index_i":  "operator[] sum over 3 parallel vector<int>[8192], 200x (SoA index loop)",
    "cs_vec_end_i":    "iterator traversal of vector<int>[8192], 200x (end() as the bound)",
    "cs_vec_push_int": "build vector<int> via push_back, 4096 elems, 80x",
    "cs_vec_push_H":   "build vector<H{7B+std::string}> via push_back, 1024 elems, 40x",
    "cs_vec_insert_H": "insert-at-begin into vector<H{string}>, 512 elems, 60x",
    "cs_vec_erase_H":  "erase-at-begin from vector<H{string}>[512] to empty, 60x",
    "cs_vec_sort_H":   "copy-construct 2048 H{string} + std::sort by string key, 60x",
    "cs_str_append":   "build std::string via += char, 4096 chars, 200x",
}
SPEED_OPS = list(SPEED_WL)


def cg_profile(binary, arg):
    """Run one workload under callgrind. Return (total_Ir, [(Ir, func_label), ...])
    self-Ir per function, descending — the attribution that says WHERE time goes."""
    out = "/tmp/cg_%s_%s.out" % (os.path.basename(binary), arg)
    sh(["valgrind", "--tool=callgrind", "--cache-sim=no", "--callgrind-out-file=" + out, binary, arg])
    total = 0
    try:
        for line in open(out):
            if line.startswith("summary:"):
                total = int(line.split()[1]); break
    except OSError:
        pass
    agg = {}
    r = sh(["callgrind_annotate", "--threshold=98", "--inclusive=no", "--auto=no", out])
    for line in r.stdout.splitlines():
        if "=>" in line or "<=" in line:          # caller/callee breakdown lines — skip
            continue
        m = re.match(r"^\s*([\d,]+)\s+(?:\([\d.]+%\)\s+)?(.+?)\s+\[", line)
        if m:
            label = m.group(2).strip()
            fn = label.split(":", 1)[1] if ":" in label else label   # drop file: prefix
            for noise in ("std::", "__gnu_cxx::", " [clone .isra.0]", " const"):
                fn = fn.replace(noise, "")
            fn = fn.strip()[:46]
            agg[fn] = agg.get(fn, 0) + int(m.group(1).replace(",", ""))   # dedupe clones
    funcs = sorted(((ir, fn) for fn, ir in agg.items()), reverse=True)
    return total, funcs


def module_speed(log):
    if not (shutil.which("valgrind") and shutil.which("callgrind_annotate")):
        log.append("  speed: callgrind absent — speed axis skipped (apt-get install valgrind)")
        return None
    src = os.path.join(ROOT, "bench", "speedprobe.cpp")
    data = {}
    for lib, cflags, lflags in (("libcis", LIBCIS_C, LIBCIS_L), ("ref", REF_C, REF_L)):
        out = "/tmp/speed_%s" % lib
        r = sh([CXX] + BASE_CXXFLAGS + cflags + [src] + lflags + ["-o", out])
        if r.returncode != 0:
            log.append("  speed %s: BUILD FAILED\n%s" % (lib, r.stdout[-1200:]))
            return None
        noop, _ = cg_profile(out, "noop")
        for op in SPEED_OPS:
            total, funcs = cg_profile(out, op)
            data.setdefault(op, {})[lib] = (total - noop, funcs)
    # full attribution to disk; digest keeps only the headline contributors
    with open(os.path.join(RESULTS, "speed_attribution.txt"), "w") as f:
        for op in SPEED_OPS:
            f.write("\n%s\n  workload: %s\n" % (op, SPEED_WL[op]))
            for lib in ("libcis", "ref"):
                if op in data and lib in data[op]:
                    tot, funcs = data[op][lib]
                    f.write("  %-6s total Ir=%d  top functions (self Ir):\n" % (lib, tot))
                    for ir, fn in funcs[:8]:
                        pct = 100.0 * ir / tot if tot else 0
                        f.write("           %12d  %4.1f%%  %s\n" % (ir, pct, fn))
    return data


# ---------------------------------------------------------------- driver
def main():
    fresh_results()
    log = []
    present, skipped = [], []
    for t in TARGETS:
        arch, cxx = t[0], t[1]
        if shutil.which(cxx[0]):
            present.append(t)
        else:
            skipped.append((arch, t[3]))

    cs = module_callsites(present, log)
    pt = module_pertype(log)
    mem = module_memory(log)
    spd = module_speed(log)

    with open(os.path.join(RESULTS, "targets.txt"), "w") as f:
        f.write("architectures this run:\n")
        measured_archs = sorted({r[0] for r in cs})
        for arch, *_ in present:
            if arch in measured_archs:
                f.write("  %-8s measured\n" % arch)
            else:
                f.write("  %-8s toolchain present but compile FAILED — see build_log.txt\n" % arch)
        for arch, hint in skipped:
            f.write("  %-8s SKIPPED — install: apt-get install %s\n" % (arch, hint))
    with open(os.path.join(RESULTS, "env.txt"), "w") as f:
        f.write("measured library: %s\n" % lib_provenance())
        f.write("repro: python3 bench/size_slope.py   (or: make size)  — ON THAT BRANCH\n")
        f.write("CXX=%s   %s\n" % (CXX, sh([CXX, "--version"]).stdout.splitlines()[0]))
        f.write("reference = host libstdc++ (non-type-erased), rebuilt every run\n")
        f.write("NOTE: numbers are specific to the branch/commit/arch above. A claim quoted\n")
        f.write("without that provenance is unverified — re-run here before believing it.\n")
    if log:
        with open(os.path.join(RESULTS, "build_log.txt"), "w") as f:
            f.write("\n".join(log))

    # ---- short summary (also printed) -------------------------------------
    prov = lib_provenance()
    out = []
    out.append("libcis size/memory/codegen — overhead vs non-type-erased reference (libstdc++)")
    out.append("MEASURED LIBRARY: %s   (re-run `make size` here to verify; do not trust prose)" % prov)
    out.append("=" * 78)

    out.append("\nPER-METHOD CODE SIZE at -Os, vs the non-type-erased reference (x86_64).")
    out.append("Size is legitimately per-call-site, so it gets a verdict. Read it method by")
    out.append("method — no aggregate pass/fail. (dsize<0 = smaller = better.)")
    sx = {op: (r[2], r[3], r[4]) for r in cs if r[0] == "x86_64" for op in [r[1]]}
    if sx:
        out.append("  %-16s %8s  %-12s %s" % ("method", "dsize", "bytes c/r", "verdict"))
        for op in CALLSITE_OPS:
            ds = sx.get(op)
            if not ds:
                continue
            dsize = ds[2]
            verdict = "smaller" if dsize < -2 else "BIGGER" if dsize > 2 else "~"
            out.append("  %-16s %+8d  %5d/%-6d %s" % (op, dsize, ds[0], ds[1], verdict))
    else:
        out.append("  (no method data — see build_log.txt)")

    # SPEED is NOT a per-method multiplier. It is per-workload, and the actionable
    # part is WHERE the instructions go. A bare ratio on an algorithm ("sort 4x")
    # hides that the cost is the element op it amortizes. Show total + attribution.
    if spd:
        out.append("\nSPEED — per workload: total instructions (Ir) and WHERE they go (callgrind).")
        out.append("Not a per-method multiplier. Full breakdown: .test_results/latest/speed_attribution.txt")
        for op in SPEED_OPS:
            d = spd.get(op, {})
            if "libcis" not in d or "ref" not in d:
                continue
            tc, fc = d["libcis"]; tr, _ = d["ref"]
            out.append("  %s   [%s]" % (op, SPEED_WL[op]))
            ratio = (" (%.1fx ref)" % (tc / tr)) if tr else ""
            out.append("     total Ir: libcis %d vs ref %d%s" % (tc, tr, ratio))
            top = ", ".join("%s %.0f%%" % (fn, 100.0 * ir / tc) for ir, fn in fc[:4] if tc)
            out.append("     libcis instructions go to: %s" % top)

    # cross-arch code-size matrix: the same verdict can differ in magnitude per ISA.
    archs = sorted({r[0] for r in cs})
    if len(archs) > 1:
        out.append("\nPER-ARCH per-call code-size overhead (libcis - that arch's libstdc++), bytes.")
        out.append("Verdicts above are x86_64; direction is usually arch-stable but size is NOT —")
        out.append("weight by your actual target ISA, not this one.")
        out.append("  %-16s %s" % ("method", "".join("%9s" % a for a in archs)))
        for op in CALLSITE_OPS:
            cells = ""
            for a in archs:
                v = next((r[4] for r in cs if r[0] == a and r[1] == op), None)
                cells += ("%+9d" % v) if v is not None else "%9s" % "-"
            if cells.strip():
                out.append("  %-16s %s" % (op, cells))

    if pt:
        out.append("\nper-type code, demoted, NOT a gate (a sanity readout, decide per method above):")
        out.append("  %+.0f B per added instantiation  (libcis %.0f vs ref %.0f)" %
                   (pt["per_type_overhead"], pt["per_type_libcis"], pt["per_type_ref"]))

    if mem:
        out.append("\nMEMORY  (representative workloads)")
        out.append("  %-9s %8s %8s %10s %10s %9s %8s" %
                   ("bench", "n", "intern", "heap_cis", "heap_ref", "heap_ovh", "stack"))
        for name, n, ii, ri, ih, rh, oh, isk, rsk in mem:
            out.append("  %-9s %8d %8d %10d %10d %+9d %8d" % (name, n, ii, ih, rh, oh, isk))

    measured_archs = sorted({r[0] for r in cs})
    failed_archs = [a for a, *_ in present if a not in measured_archs]
    out.append("\narchitectures: measured=[%s]  toolchain-present-but-failed=[%s]  skipped=[%s]" %
               (", ".join(measured_archs), ", ".join(failed_archs),
                ", ".join(a for a, _ in skipped)))
    out.append("\nFULL RESULTS in .test_results/latest/ — OPEN THESE, the numbers above are a digest:")
    out.append("  callsites/<arch>/<libcis|ref>/<op>.asm   annotated disassembly of every call site")
    out.append("  per_call_overhead.csv   per-arch per-op code size, libcis vs reference")
    out.append("  speed_attribution.txt   per-workload Ir + where the instructions go (callgrind)")
    out.append("  memory.csv              internal / heap / stack per workload")
    out.append("  per_type.csv, static.txt, targets.txt, env.txt" +
               (", build_log.txt (FAILURES)" if log else ""))

    text = "\n".join(out)
    with open(os.path.join(RESULTS, "summary.txt"), "w") as f:
        f.write(text + "\n")
    print(text)
    return 0


if __name__ == "__main__":
    sys.exit(main())
