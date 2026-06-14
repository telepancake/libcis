#!/usr/bin/env python3
"""codesize.py - report compiled code (.text) size of real STL-using projects
built against libcis with -Os.

For each driver in bench/drivers/ we compile + link it against libcis exactly
the way tools/run_std.sh links the conformance suite (freestanding, g++-10,
-std=gnu++20 -fno-exceptions -fno-rtti, -nostdinc++ -Iinclude, support.cpp +
-nodefaultlibs), but at -Os, then read the .text section size with `size -A`.

Each driver is also *run* after linking: a size number for a binary that does
not work is meaningless, so a project only counts if its driver exits 0.

The bench projects live as git submodules under bench/projects/; init them with
    git submodule update --init --depth 1

Usage:
    bench/codesize.py [--keep] [--json] [project ...]

Output is a table of .text / .rodata / .data / .bss / total bytes per project,
with the empty-program 'baseline' first and each project's marginal .text over
that baseline. Exit status is 0 iff every selected project compiled, linked and
ran; non-zero otherwise, so this doubles as a test.
"""
import argparse
import json
import os
import shutil
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DRIVERS = os.path.join(ROOT, "bench", "drivers")
PROJECTS = os.path.join(ROOT, "bench", "projects")

# Canonical target is gcc-10.2 (see CONVENTIONS.md; tools/run_std.sh hardcodes
# g++-10). Honour $CXX, else default to g++-10. We pin the compiler rather than
# probing PATH because .text differs between g++ majors (e.g. g++-13 produces
# slightly smaller code here), so comparable bench numbers need a fixed toolchain.
def pick_cxx():
    if os.environ.get("CXX"):
        return os.environ["CXX"].split()
    return ["g++-10"]

CXX = pick_cxx()
SIZE = os.environ.get("SIZE", "size")

# libcis compile + link flags, mirroring tools/run_std.sh, plus -Os.
COMPILE = ["-std=gnu++20", "-nostdinc++", "-I" + os.path.join(ROOT, "include"),
           "-fno-exceptions", "-fno-rtti", "-Os", "-w"]
LINK = [os.path.join(ROOT, "src", "support.cpp"),
        "-nodefaultlibs", "-lpthread", "-lm", "-lc", "-lgcc_s", "-lgcc"]

# Each project: header search dirs (relative to its submodule) and extra defines
# the driver needs. The driver itself is bench/drivers/<name>.cpp.
PROJECTS_CFG = {
    "baseline": {"dirs": [], "defines": []},
    "fmt": {
        "dirs": ["fmt/include"],
        "defines": [],  # driver sets FMT_HEADER_ONLY / FMT_EXCEPTIONS itself
    },
    "unordered_dense": {
        "dirs": ["unordered_dense/include"],
        "defines": [],
    },
    "magic_enum": {
        "dirs": ["magic_enum/include"],
        "defines": [],
    },
    "json": {
        "dirs": ["json/single_include"],
        "defines": [],  # driver sets JSON_NOEXCEPTION itself
    },
    "tomlplusplus": {
        "dirs": ["tomlplusplus/include"],
        "defines": [],  # driver sets TOML_EXCEPTIONS 0 itself
    },
    "doctest": {
        "dirs": ["doctest"],
        "defines": [],  # driver sets the no-exceptions config itself
    },
}

ORDER = ["baseline", "fmt", "unordered_dense", "magic_enum",
         "json", "tomlplusplus", "doctest"]


def run(cmd, **kw):
    return subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                          **kw)


def section_sizes(binary):
    """Return {.text, .rodata, .data, .bss, total} from `size -A`."""
    out = run([SIZE, "-A", binary]).stdout.decode("utf-8", "replace")
    secs = {}
    for line in out.splitlines():
        parts = line.split()
        if len(parts) >= 2 and parts[0].startswith(".") and parts[1].isdigit():
            secs[parts[0]] = int(parts[1])
    return {
        "text": secs.get(".text", 0),
        "rodata": secs.get(".rodata", 0),
        "data": secs.get(".data", 0),
        "bss": secs.get(".bss", 0),
    }


def build_one(name, cfg, workdir, keep):
    driver = os.path.join(DRIVERS, name + ".cpp")
    if not os.path.exists(driver):
        return {"name": name, "ok": False, "stage": "driver", "msg": "no driver"}
    incs = []
    for d in cfg["dirs"]:
        p = os.path.join(PROJECTS, d)
        if not os.path.isdir(p):
            return {"name": name, "ok": False, "stage": "submodule",
                    "msg": f"missing {d} (run: git submodule update --init)"}
        incs.append("-I" + p)
    defines = ["-D" + d for d in cfg["defines"]]
    out = os.path.join(workdir, name)

    cmd = CXX + COMPILE + incs + defines + [driver] + LINK + ["-o", out]
    r = run(cmd)
    if r.returncode != 0:
        return {"name": name, "ok": False, "stage": "compile",
                "msg": r.stdout.decode("utf-8", "replace")}

    rr = run([out], timeout=30)
    if rr.returncode != 0:
        return {"name": name, "ok": False, "stage": "run",
                "msg": f"exit {rr.returncode}"}

    sizes = section_sizes(out)
    if keep:
        shutil.copy(out, os.path.join(ROOT, "bench", name + ".bin"))
    return {"name": name, "ok": True, **sizes}


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("projects", nargs="*", help="subset to build (default: all)")
    ap.add_argument("--keep", action="store_true",
                    help="leave built binaries in bench/")
    ap.add_argument("--json", action="store_true", help="emit JSON")
    args = ap.parse_args()

    if not shutil.which(CXX[0]):
        print(f"compiler not found: {CXX[0]} (libcis targets g++-10; set $CXX "
              f"to override)", file=sys.stderr)
        return 2

    names = args.projects or ORDER
    workdir = tempfile.mkdtemp(prefix="codesize_")
    results = []
    try:
        for name in names:
            cfg = PROJECTS_CFG.get(name)
            if cfg is None:
                print(f"unknown project: {name}", file=sys.stderr)
                results.append({"name": name, "ok": False, "stage": "config",
                                "msg": "unknown"})
                continue
            results.append(build_one(name, cfg, workdir, args.keep))
    finally:
        shutil.rmtree(workdir, ignore_errors=True)

    if args.json:
        print(json.dumps(results, indent=2))
    else:
        base = next((r["text"] for r in results
                     if r.get("name") == "baseline" and r["ok"]), 0)
        print(f"libcis code size @ -Os   (CXX={' '.join(CXX)})")
        print(f"{'project':<18}{'.text':>10}{'+base':>10}{'.rodata':>10}"
              f"{'.data':>8}{'.bss':>8}")
        print("-" * 64)
        for r in results:
            if not r["ok"]:
                print(f"{r['name']:<18}{'FAIL (' + r['stage'] + ')':>10}")
                continue
            marg = "" if r["name"] == "baseline" else f"+{r['text'] - base}"
            print(f"{r['name']:<18}{r['text']:>10}{marg:>10}{r['rodata']:>10}"
                  f"{r['data']:>8}{r['bss']:>8}")

    failed = [r for r in results if not r["ok"]]
    if failed:
        print("\nFAILURES:", file=sys.stderr)
        for r in failed:
            msg = r.get("msg", "").strip().splitlines()
            tail = msg[-1] if msg else ""
            print(f"  {r['name']} @ {r['stage']}: {tail}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
