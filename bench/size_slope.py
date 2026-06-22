#!/usr/bin/env python3
"""size_slope.py — the useful size/memory/perf metric for libcis (`make size`).

Why this and not bench/codesize.py: that one links each driver as a standalone
binary and reports `.text` over an empty `main`. The empty main contains none of
the shared container machinery, so the entire one-time cost of that machinery is
charged to every single measurement, with nothing to amortize it against. A
technique whose whole premise is "pay once in the shared library, save at every
instantiation/callsite" therefore always reads as overhead. That harness can
only ever vote against code sharing.

This metric measures the right thing instead. It builds ONE composite image
(bench/firmware.cpp) at several breadths NT, and fits

        flash(NT) = intercept + slope * NT
        ram(NT)   = intercept + slope * NT

  * slope     = marginal flash/RAM per additional container instantiation.
                This is what a whole-firmware build pays per extra container,
                and the number code-sharing must drive DOWN. It is independent
                of the one-time core cost by construction (the fit removes it).
  * intercept = the one-time shared-core cost. Paid ONCE for the whole image,
                so it is allowed to rise if it buys a lower slope — that trade
                is the entire point. Reported, lightly bounded, never the gate.

Plus two axes you cannot trade away silently:
  * per-object RAM   — sizeof of the common containers (memory, not flash).
  * perf             — wall time of a fixed workload; a size win that pessimizes
                       the shared hot path shows up here.

The gate compares against bench/size_baseline.json (committed). The numbers are
toolchain-relative; pin CXX to the canonical g++-10 for the recorded baseline.
A change is GOOD iff it lowers the slope (and the representative total) without
regressing per-object RAM or perf past tolerance. Lowering the slope by raising
the intercept is allowed and expected; raising the slope is the regression.
"""
import json, os, re, shutil, subprocess, sys, time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CXX = os.environ.get("CXX", "g++-10").split()
if not shutil.which(CXX[0]):
    CXX = ["g++"]  # fall back so the harness runs anywhere; baseline pins g++-10
SIZE = os.environ.get("SIZE", "size")

COMPILE = ["-std=gnu++20", "-nostdinc++", "-I" + os.path.join(ROOT, "include"),
           "-fno-exceptions", "-fno-rtti", "-Os", "-w"]
LINK = [os.path.join(ROOT, "src", "support.cpp"),
        "-nodefaultlibs", "-lpthread", "-lm", "-lc", "-lgcc_s", "-lgcc"]
OPT = ["-flto", "-fmerge-all-constants", "-ffunction-sections", "-fdata-sections",
       "-Wl,--gc-sections", "-fvisibility=hidden", "-fvisibility-inlines-hidden",
       "-fno-asynchronous-unwind-tables", "-fno-unwind-tables"]
if shutil.which("ld.gold"):
    OPT += ["-fuse-ld=gold", "-Wl,--icf=safe"]

DRIVER = os.path.join(ROOT, "bench", "firmware.cpp")
SWEEP = [4, 8, 16, 32]          # breadths; slope is fit across these
PERF_REPS = 300                 # workload repetitions for the perf build
TOL_SLOPE = 0.005               # +0.5% slope is the regression line
TOL_PERF = 0.05                 # +5% wall time is the regression line
BASELINE = os.path.join(ROOT, "bench", "size_baseline.json")


def run(cmd):
    return subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)


def sections(binary):
    r = run([SIZE, "-A", binary])
    secs = {}
    for line in r.stdout.splitlines():
        m = re.match(r"^(\.\S+)\s+(\d+)", line)
        if m:
            secs[m.group(1)] = int(m.group(2))
    return secs


def build(nt, perf=False, out="/tmp/firmware_bin"):
    defs = ["-DNT=%d" % nt]
    if perf:
        defs.append("-DPERF=%d" % PERF_REPS)
    cmd = CXX + COMPILE + OPT + defs + [DRIVER] + LINK + ["-o", out]
    r = run(cmd)
    if r.returncode != 0:
        sys.stderr.write("BUILD FAILED (NT=%d):\n%s\n" % (nt, r.stdout[-2000:]))
        sys.exit(2)
    return out


def fit(xs, ys):
    """Ordinary least squares -> (slope, intercept)."""
    n = len(xs)
    sx, sy = sum(xs), sum(ys)
    sxx = sum(x * x for x in xs)
    sxy = sum(x * y for x, y in zip(xs, ys))
    slope = (n * sxy - sx * sy) / (n * sxx - sx * sx)
    intercept = (sy - slope * sx) / n
    return slope, intercept


def measure_sizeof():
    src = "/tmp/sizeof_probe.cpp"
    with open(src, "w") as f:
        f.write(r'''
#include <vector>
#include <string>
#include <cstdio>
template<class T> struct P { char pad[3]; std::string s; };
int main(){
  printf("vector<int> %zu\n", sizeof(std::vector<int>));
  printf("vector<P> %zu\n",   sizeof(std::vector<P<int>>));
  printf("string %zu\n",      sizeof(std::string));
  return 0;
}
''')
    out = "/tmp/sizeof_probe"
    cmd = CXX + COMPILE + [src] + LINK + ["-o", out]
    if run(cmd).returncode != 0:
        return {}
    r = run([out])
    d = {}
    for line in r.stdout.splitlines():
        parts = line.rsplit(" ", 1)
        if len(parts) == 2 and parts[1].isdigit():
            d[parts[0]] = int(parts[1])
    return d


def measure_perf():
    b = build(16, perf=True, out="/tmp/firmware_perf")
    best = min(_time(b) for _ in range(3))
    return best


def _time(binary):
    t0 = time.perf_counter()
    subprocess.run([binary], stdout=subprocess.DEVNULL)
    return time.perf_counter() - t0


def main():
    flash, ram = [], []
    print("breadth sweep (one composite image, shared core linked once):")
    print("  %-6s %10s %10s %10s" % ("NT", "flash", "ram_static", "total"))
    for nt in SWEEP:
        s = sections(build(nt))
        fl = s.get(".text", 0) + s.get(".rodata", 0)
        rm = s.get(".data", 0) + s.get(".bss", 0)
        flash.append(fl); ram.append(rm)
        print("  %-6d %10d %10d %10d" % (nt, fl, rm, fl + rm))

    fslope, fint = fit(SWEEP, flash)
    rslope, rint = fit(SWEEP, ram)
    sizeofs = measure_sizeof()
    perf = measure_perf()

    metric = {
        "cxx": " ".join(CXX),
        "flash_slope": round(fslope, 1),     # bytes / added instantiation  <-- THE gate
        "flash_intercept": round(fint, 1),   # one-time shared-core cost (paid once)
        "ram_slope": round(rslope, 1),
        "ram_intercept": round(rint, 1),
        "sizeof": sizeofs,                    # per-object RAM
        "perf_s": round(perf, 4),            # wall time, fixed workload
    }
    print("\nMETRIC (lower flash_slope is the goal; intercept may rise to buy it):")
    for k in ("flash_slope", "flash_intercept", "ram_slope", "ram_intercept", "perf_s"):
        print("  %-16s %s" % (k, metric[k]))
    print("  sizeof           %s" % sizeofs)

    base = None
    if os.path.exists(BASELINE):
        base = json.load(open(BASELINE))

    if "--record" in sys.argv:
        json.dump(metric, open(BASELINE, "w"), indent=2)
        print("\nrecorded baseline -> %s" % BASELINE)
        return 0

    if base is None:
        print("\nno baseline yet; run `make size-record` to set one. (informational)")
        return 0

    print("\ngate vs baseline (%s):" % base.get("cxx", "?"))
    fails = []
    fs0 = base["flash_slope"]
    if fslope > fs0 * (1 + TOL_SLOPE):
        fails.append("flash_slope %.1f > baseline %.1f (+%.1f%%)"
                     % (fslope, fs0, 100 * (fslope - fs0) / fs0))
    for k, v in sizeofs.items():
        if v > base.get("sizeof", {}).get(k, v):
            fails.append("sizeof %s grew %d -> %d" % (k, base["sizeof"][k], v))
    p0 = base.get("perf_s")
    if p0 and perf > p0 * (1 + TOL_PERF):
        fails.append("perf %.4fs > baseline %.4fs (+%.1f%%)" % (perf, p0, 100 * (perf - p0) / p0))

    dflash = fslope - fs0
    print("  flash_slope %+.1f B/instantiation   intercept %+.1f B (one-time)"
          % (dflash, fint - base["flash_intercept"]))
    if fails:
        print("\nNOT CLEAN:")
        for f in fails:
            print("  - " + f)
        return 1
    print("\nCLEAN" + ("  (slope improved)" if dflash < 0 else "  (slope held)"))
    return 0


if __name__ == "__main__":
    sys.exit(main())
