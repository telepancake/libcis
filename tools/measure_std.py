#!/usr/bin/env python3
"""measure_std.py - Phase-1 vendor measurement of the AST-transferred suite.

For every transferred test (test/std/, from tools/transfer.py) this compiles
(and, for run-kind tests, runs) the file against the TWO VENDOR toolchains:

  libcxx     : clang++-20 -std=gnu++2c -stdlib=libc++
  libstdcxx  : g++-14     -std=gnu++26

It does NOT touch libcis (that is the next phase).  Results are tallied
per-subtree per-vendor and written incrementally to test/std/results.json after
every subtree, so a partial / interrupted run still yields a real, honest table
(no extrapolation).  Per-subtree, per-vendor we record:

  inputs transferred skipped errors compile_pass compile_fail run_pass run_fail

`inputs/skipped/errors` come from manifest.json (the transfer stage); the
compile/run numbers are measured here.  A *.compile.pass.cpp (kind=compile) is
-fsyntax-only and never run.  A run-kind test is linked with a generated driver
that calls test_<slug>() and executed with a 30s timeout.

Usage:
  tools/measure_std.py [--jobs N] [--vendors libcxx,libstdcxx] [SUBTREE ...]
    (default: measure every subtree present in the manifest)
"""
import argparse
import json
import multiprocessing as mp
import os
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DST_ROOT = os.path.join(ROOT, "test", "std")
SUPPORT = os.path.join(DST_ROOT, "support")
RESULTS = os.path.join(DST_ROOT, "results.json")

COMMON = ["-fno-exceptions", "-fno-rtti", "-O0", "-w", "-I" + SUPPORT]

VENDORS = {
    "libcxx": {"cc": ["clang++-20", "-std=gnu++2c", "-stdlib=libc++"], "link": ["-pthread"]},
    "libstdcxx": {"cc": ["g++-14", "-std=gnu++26"], "link": ["-pthread"]},
}


def subtree_of(rel):
    return rel.split("/", 1)[0]


def measure_one(task):
    rel, slug, kind, flags, vendor, workdir = task
    v = VENDORS[vendor]
    src = os.path.join(DST_ROOT, rel)
    tag = rel.replace("/", "_")
    base = v["cc"] + COMMON + flags
    res = {"rel": rel, "vendor": vendor, "kind": kind}
    if kind == "compile":
        cmd = base + ["-fsyntax-only", src]
        rc = run(cmd)
        res["compile"] = (rc == 0)
        res["run"] = None
        return res
    # run-kind: link with a driver, then execute
    out = os.path.join(workdir, f"{tag}.{vendor}.bin")
    drv = os.path.join(workdir, f"{tag}.{vendor}.driver.cpp")
    with open(drv, "w") as fh:
        fh.write(f"void test_{slug}();\nint main(){{ test_{slug}(); return 0; }}\n")
    cmd = base + [src, drv] + v["link"] + ["-o", out]
    rc = run(cmd)
    if rc != 0:
        res["compile"] = False
        res["run"] = None
        return res
    res["compile"] = True
    res["run"] = (run([out], timeout=30) == 0)
    if os.path.exists(out):
        try:
            os.remove(out)
        except OSError:
            pass
    if os.path.exists(drv):
        try:
            os.remove(drv)
        except OSError:
            pass
    return res


def run(cmd, timeout=120):
    try:
        p = subprocess.run(cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
                           timeout=timeout)
        return p.returncode
    except subprocess.TimeoutExpired:
        return 124
    except Exception:
        return 125


def load_results():
    if os.path.exists(RESULTS):
        with open(RESULTS) as fh:
            return json.load(fh)
    return {}


def save_results(r):
    tmp = RESULTS + ".tmp"
    with open(tmp, "w") as fh:
        json.dump(r, fh, indent=1)
    os.replace(tmp, RESULTS)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("subtrees", nargs="*")
    ap.add_argument("-j", "--jobs", type=int, default=max(1, (os.cpu_count() or 1)))
    ap.add_argument("--vendors", default="libcxx,libstdcxx")
    args = ap.parse_args()
    vendors = [v for v in args.vendors.split(",") if v]

    with open(os.path.join(DST_ROOT, "manifest.json")) as fh:
        man = json.load(fh)

    # group transferred by subtree
    by_sub = {}
    for r in man["transferred"]:
        by_sub.setdefault(subtree_of(r["file"]), []).append(r)
    skip_count = {}
    err_count = {}
    inp_count = {}
    for r in man["skipped"]:
        skip_count[subtree_of(r["file"])] = skip_count.get(subtree_of(r["file"]), 0) + 1
    for r in man["errors"]:
        err_count[subtree_of(r["file"])] = err_count.get(subtree_of(r["file"]), 0) + 1
    for k in set(list(by_sub) + list(skip_count) + list(err_count)):
        inp_count[k] = (len(by_sub.get(k, [])) + skip_count.get(k, 0)
                        + err_count.get(k, 0))

    subs = args.subtrees or sorted(by_sub)
    results = load_results()

    workdir = tempfile.mkdtemp(prefix="measure_std_")
    for sub in subs:
        rows = by_sub.get(sub, [])
        if not rows:
            continue
        sub_res = results.setdefault(sub, {})
        sub_res["inputs"] = inp_count.get(sub, 0)
        sub_res["transferred"] = len(rows)
        sub_res["skipped"] = skip_count.get(sub, 0)
        sub_res["errors"] = err_count.get(sub, 0)
        for vendor in vendors:
            tasks = [(r["file"], r["slug"], r["kind"], r.get("flags", []),
                      vendor, workdir) for r in rows]
            cp = cf = rp = rf = 0
            cfails = []
            rfails = []
            with mp.Pool(processes=args.jobs) as pool:
                for res in pool.imap_unordered(measure_one, tasks, chunksize=2):
                    if res["compile"]:
                        cp += 1
                    else:
                        cf += 1
                        cfails.append(res["rel"])
                    if res["run"] is True:
                        rp += 1
                    elif res["run"] is False:
                        rf += 1
                        rfails.append(res["rel"])
            sub_res[vendor] = {
                "compile_pass": cp, "compile_fail": cf,
                "run_pass": rp, "run_fail": rf,
                "compile_fails": sorted(cfails)[:50],
                "run_fails": sorted(rfails)[:50],
            }
            save_results(results)
            print(f"[{sub}/{vendor}] compile {cp}/{cp+cf}  run-pass {rp}/{rp+rf}",
                  flush=True)
    print("MEASURE_DONE", flush=True)


if __name__ == "__main__":
    main()
