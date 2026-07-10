#!/usr/bin/env python3
"""Run transferred libc++ tests INDIVIDUALLY against the LEAN overlay.

The lean twin of tools/run_files.py: same corpus (test/std/, produced by the
base transfer — the transfer is library-agnostic), same binary CLEAN/NOT-CLEAN
contract, different compile shape:

    -Ilean/include -Iinclude          (the overlay: lean wins, base falls through)
    + build/lean/libleansupport.a     (support.cpp + kernels.cpp, prebuilt once)

Exclusions are TWO layers, both justified files:
  tools/exclusions.json        base layer (compiler limits, target-impossible)
  lean/tools/exclusions.json   lean layer (documented lean deviations only);
                               entries: exact file, 'prefix/' subtree, 'glob:pat'

State is binary: the lean profile is correct iff EVERY non-excluded test
passes. Failures are the work queue — fix the lean bug, or move the test into
lean/tools/exclusions.json with a justification tying it to a documented
deviation. No ratios, no silent skips.

Usage: lean/tools/run_std.py <subtree-prefix> [limit] [-jN] [--base]

--base runs the same tests against the BASE library instead (base include
order, base exclusions only): the diff between a --base run and a lean run of
the same subtree is the set of LEAN-ONLY failures — the lean work queue. A
test failing on both is a base-level concern, not a lean regression.

lean/tools/exclusions.d/*.json fragments are merged into the lean exclusion
layer (same schema), so parallel subtree work never contends on one file.
"""
import fnmatch
import json
import multiprocessing
import os
import re
import subprocess
import sys
import collections

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
os.chdir(ROOT)
sys.path.insert(0, os.path.join(ROOT, "tools"))
import config as cfg  # noqa: E402

CXX = cfg.require_cxx(cfg.CXX_LIBCIS, "libcis")
BASE_MODE = "--base" in sys.argv
INC = " -Iinclude" if BASE_MODE else " -Ilean/include -Iinclude"
FLAGS = (" -std=gnu++20 -fcoroutines -fno-exceptions -fno-rtti -nostdinc++"
         + INC + " -Itest/std -Itest/std/support -O0 -w").split()
SUPLIB = ("build/lean/libbasesupport.a" if BASE_MODE
          else "build/lean/libleansupport.a")
LINK = [SUPLIB] + "-nodefaultlibs -lpthread -lm -lc -lgcc_s -lgcc".split()

# Re-apply tools/test_overrides exactly as tools/run_files.py does (the
# transfer regenerates test/std and would silently revert the hand-adapted
# gcc-frontend-defect workarounds; the override files carry the evidence).
def apply_overrides():
    ovr = os.path.join(ROOT, "tools", "test_overrides")
    slug = {}
    man_path = os.path.join(ROOT, "test/std/manifest.json")
    if os.path.exists(man_path):
        slug = {r["file"]: r["slug"]
                for r in json.load(open(man_path))["transferred"] if r.get("slug")}
    for dirp, _, files in os.walk(ovr):
        for fn in files:
            src = os.path.join(dirp, fn)
            rel = os.path.relpath(src, ovr)
            dst = os.path.join(ROOT, "test/std", rel)
            text = open(src).read()
            s = slug.get(rel)
            if s:
                text = re.sub(r"libcis_ns_\w+", "libcis_ns_" + s, text)
            if os.path.exists(dst) and open(dst).read() != text:
                open(dst, "w").write(text)


def build_support():
    """One archive with support.o + kernels.o so per-test links are cheap
    (mirrors how gen_groups builds libsupport.a for the base backend)."""
    os.makedirs("build/lean", exist_ok=True)
    objs = []
    srcs = ("src/support.cpp",) if BASE_MODE else ("src/support.cpp", "lean/src/kernels.cpp")
    for src in srcs:
        obj = os.path.join("build/lean",
                           ("base_" if BASE_MODE else "") + os.path.basename(src) + ".o")
        if (not os.path.exists(obj)
                or os.path.getmtime(obj) < os.path.getmtime(src)):
            p = subprocess.run([CXX] + FLAGS + ["-c", src, "-o", obj],
                               capture_output=True, text=True)
            if p.returncode != 0:
                sys.exit(f"support build failed for {src}:\n{p.stderr[-2000:]}")
        objs.append(obj)
    subprocess.run(["ar", "rcs", SUPLIB] + objs, check=True)


class Exclusions:
    def __init__(self):
        import glob as globmod
        layers = [json.load(open("tools/exclusions.json"))]
        if not BASE_MODE:
            layers.append(json.load(open("lean/tools/exclusions.json")))
            for frag in sorted(globmod.glob("lean/tools/exclusions.d/*.json")):
                layers.append(json.load(open(frag)))
        keys = [k for layer in layers for k in layer]
        self.exact, self.prefixes, self.globs = set(), [], []
        for k in keys:
            if k.startswith("_"):
                continue
            if k.startswith("glob:"):
                self.globs.append(k[5:])
            elif k.endswith("/"):
                self.prefixes.append(k)
            else:
                self.exact.add(k)

    def __contains__(self, f):
        return (f in self.exact
                or any(f.startswith(p) for p in self.prefixes)
                or any(fnmatch.fnmatch(f, g) for g in self.globs))


def extra_flags(src_path):
    """ADDITIONAL_COMPILE_FLAGS, minus lit substitutions we don't expand here
    (tools/run_files.py has the locale machinery; the lean gate drops flag
    lines containing unexpanded %{...} the same way rather than mis-passing
    them)."""
    flags = []
    with open(src_path, errors="replace") as fh:
        for line in fh:
            s = line.strip()
            if s.startswith("int main") or s.startswith("#include"):
                break
            if "ADDITIONAL_COMPILE_FLAGS:" not in s:
                continue
            spec = s.split("ADDITIONAL_COMPILE_FLAGS:", 1)[1].strip()
            if "%{" in spec:
                continue
            flags += spec.split()
    return flags


def run_one(r):
    src = os.path.abspath(os.path.join("test/std", r["file"]))
    tag = f"{os.getpid()}_{abs(hash(r['file'])) % (1 << 30)}"
    drv, exe = f"/tmp/lean_std_{tag}.cpp", f"/tmp/lean_std_{tag}"
    entry = r["entry"]
    if entry.endswith("(0, nullptr)"):
        fn = entry[: -len("(0, nullptr)")]
        entry = f"cis_call_main(&{fn}, argc, argv)"
    open(drv, "w").write(
        f'#include "{src}"\n'
        "template<class R> R cis_call_main(R(*f)(int, char**), int c, char** v)"
        "{ return f(c, v); }\n"
        "template<class R> R cis_call_main(R(*f)(int, const char**), int c, char** v)"
        "{ return f(c, const_cast<const char**>(v)); }\n"
        f"int main(int argc, char** argv){{ return {entry}; }}\n")
    try:
        try:
            p = subprocess.run([CXX] + FLAGS + extra_flags(src) + [drv] + LINK
                               + ["-o", exe],
                               capture_output=True, text=True, timeout=180)
        except subprocess.TimeoutExpired:
            return (r["file"], "compile", "compile-timeout (180s)")
        if p.returncode != 0:
            m = re.search(r"error: (.+)", p.stderr)
            return (r["file"], "compile", (m.group(1)[:110] if m else "?"))
        try:
            rc = subprocess.run([exe], capture_output=True, timeout=60).returncode
        except subprocess.TimeoutExpired:
            rc = -99
        return None if rc == 0 else (r["file"], "run", f"exit={rc}")
    finally:
        for f in (drv, exe):
            try:
                os.unlink(f)
            except OSError:
                pass


def main():
    args = [a for a in sys.argv[1:]
            if not a.startswith("-j") and a != "--base"]
    jobs = next((int(a[2:]) for a in sys.argv[1:] if a.startswith("-j")),
                max(1, (os.cpu_count() or 2) - 1))
    if not args:
        sys.exit("usage: lean/tools/run_std.py <subtree-prefix> [limit] [-jN]")
    pre = args[0]
    limit = int(args[1]) if len(args) > 1 else 10 ** 9

    apply_overrides()
    build_support()
    excl = Exclusions()
    man = json.load(open("test/std/manifest.json"))
    tests = [r for r in man["transferred"]
             if r["file"].startswith(pre) and r["kind"] == "run"
             and r.get("entry") and r["file"] not in excl][:limit]
    excluded = sum(1 for r in man["transferred"]
                   if r["file"].startswith(pre) and r["kind"] == "run"
                   and r.get("entry") and r["file"] in excl)
    print(f"{'base' if BASE_MODE else 'lean'} gate: {len(tests)} tests under '{pre}' "
          f"(excluded(justified)={excluded}, -j{jobs})", flush=True)

    with multiprocessing.Pool(jobs) as pool:
        results = pool.map(run_one, tests, chunksize=4)
    fails = [x for x in results if x]
    cerr = collections.Counter(e for _, k, e in fails if k == "compile")
    for f, kind, err in sorted(fails):
        print(f"FAIL({kind:7s}) {f}" + (f"\n              {err}" if kind == "compile" else f"  {err}"))
    if cerr:
        print("-- compile-error census (top 10):")
        for e, c in cerr.most_common(10):
            print(f"   {c:4d}  {e}")
    print("CLEAN" if not fails else f"NOT CLEAN ({len(fails)} failing / {len(tests)})")
    sys.exit(0 if not fails else 1)


if __name__ == "__main__":
    main()
