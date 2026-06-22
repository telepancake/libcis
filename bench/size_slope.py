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
SPEED_OPS = ["cs_vec_index_i", "cs_vec_end_i", "cs_vec_push_int", "cs_vec_push_H",
             "cs_vec_insert_H", "cs_vec_erase_H", "cs_vec_sort_H", "cs_str_append"]


def cg_irefs(binary, arg):
    """Deterministic instruction reads (Ir) for one workload, via cachegrind."""
    out = "/tmp/cg_%s.out" % os.path.basename(binary)
    r = sh(["valgrind", "--tool=cachegrind", "--cachegrind-out-file=" + out, binary, arg])
    m = re.search(r"I\s+refs:\s+([\d,]+)", r.stdout)
    return int(m.group(1).replace(",", "")) if m else None


def module_speed(log):
    if not shutil.which("valgrind"):
        log.append("  speed: valgrind absent — speed axis skipped (apt-get install valgrind)")
        return None
    src = os.path.join(ROOT, "bench", "speedprobe.cpp")
    res = {}
    for lib, cflags, lflags in (("libcis", LIBCIS_C, LIBCIS_L), ("ref", REF_C, REF_L)):
        out = "/tmp/speed_%s" % lib
        r = sh([CXX] + BASE_CXXFLAGS + cflags + [src] + lflags + ["-o", out])
        if r.returncode != 0:
            log.append("  speed %s: BUILD FAILED\n%s" % (lib, r.stdout[-1200:]))
            return None
        base = cg_irefs(out, "noop")            # subtract fixed startup
        per = {}
        for op in SPEED_OPS:
            ir = cg_irefs(out, op)
            if ir is not None and base is not None:
                per[op] = ir - base             # method's own executed instructions
        res[lib] = per
    with open(os.path.join(RESULTS, "speed.csv"), "w", newline="") as f:
        w = csv.writer(f); w.writerow(["op", "ir_libcis", "ir_ref", "pct"])
        for op in SPEED_OPS:
            lc, rf = res.get("libcis", {}).get(op), res.get("ref", {}).get(op)
            if lc is not None and rf:
                w.writerow([op, lc, rf, "%.1f" % (100 * (lc - rf) / rf)])
    return res


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
        f.write("repro: python3 bench/size_slope.py   (or: make size)\n")
        f.write("CXX=%s   %s\n" % (CXX, sh([CXX, "--version"]).stdout.splitlines()[0]))
        f.write("reference = host libstdc++ (non-type-erased), rebuilt every run\n")
    if log:
        with open(os.path.join(RESULTS, "build_log.txt"), "w") as f:
            f.write("\n".join(log))

    # ---- short summary (also printed) -------------------------------------
    out = []
    out.append("libcis size/memory/codegen — overhead vs non-type-erased reference (libstdc++)")
    out.append("=" * 78)

    out.append("\nPER-METHOD: code size AND speed at -Os, vs the non-type-erased reference.")
    out.append("Read this method by method. There is NO aggregate pass/fail — a change that")
    out.append("helps some methods and hurts others is half-right: keep the wins, fix the")
    out.append("regressions, individually. WIN = better on an axis, worse on none. LOSS = the")
    out.append("reverse. TRADE = better on one, worse on the other → a human decides if the")
    out.append("win there is worth the loss here. (dsize<0 and dspeed<0 are improvements.)")
    out.append("speed = cachegrind instruction reads (Ir, deterministic), startup subtracted.")
    sx = {op: (r[2], r[3], r[4]) for r in cs if r[0] == "x86_64" for op in [r[1]]}
    sp_c = (spd or {}).get("libcis", {}); sp_r = (spd or {}).get("ref", {})
    if sx:
        out.append("  %-16s %7s %-11s %8s %-15s  %s" %
                   ("method", "dsize", "size c/r", "dspeed", "kIr c/r", "verdict"))
        for op in CALLSITE_OPS:
            if op not in sx and op not in sp_c:
                continue
            ds = sx.get(op)
            dpc, dpr = sp_c.get(op), sp_r.get(op)
            dsize = ds[2] if ds else None
            dspd = (100 * (dpc - dpr) / dpr) if (dpc and dpr) else None
            scol = ("%+7d %5d/%-5d" % (dsize, ds[0], ds[1])) if ds else "%7s %-11s" % ("-", "")
            pcol = ("%+7.1f%% %6d/%-7d" % (dspd, dpc // 1000, dpr // 1000)) if dspd is not None else "%8s %-15s" % ("-", "")
            # verdict from deadbanded axes (Ir is deterministic -> tight 1% band)
            better = (dsize is not None and dsize < -2) or (dspd is not None and dspd < -1)
            worse = (dsize is not None and dsize > 2) or (dspd is not None and dspd > 1)
            verdict = "TRADE" if better and worse else "WIN" if better else "LOSS" if worse else "~"
            out.append("  %-16s %s %s  %s" % (op, scol, pcol, verdict))
    else:
        out.append("  (no method data — see build_log.txt)")

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
    out.append("  speed.csv               per-method ns/op at -Os, libcis vs reference")
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
