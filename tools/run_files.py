#!/usr/bin/env python3
"""Run each transferred test of a subtree INDIVIDUALLY against a backend.

The consolidated feature-group TU is all-or-nothing: one compile error blanks
every test's signal.  For the foundational headers (string, vector, the big
containers) that is exactly what has hidden their real status.  This compiles
and runs each file on its own -> per-file pass / compile-fail / run-fail, with
the first error of each failure bucketed, so a buggy vector can't hide behind a
TU that won't compile.

Usage: tools/run_files.py <subtree-prefix> [limit]
"""
import json
import os
import re
import subprocess
import sys
import collections

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.chdir(ROOT)

CIS = ("g++-10 -std=gnu++20 -fno-exceptions -fno-rtti -nostdinc++ -Iinclude "
       "-Itest/std -Itest/std/support -O0 -w").split()
LINK = "build/groups/libcis/libsupport.a -nodefaultlibs -lpthread -lm -lc -lgcc_s -lgcc".split()

pre = sys.argv[1]
limit = int(sys.argv[2]) if len(sys.argv) > 2 else 10**9
man = json.load(open("test/std/manifest.json"))
tests = [r for r in man["transferred"]
         if r["file"].startswith(pre) and r["kind"] == "run" and r.get("entry")][:limit]

buckets = collections.Counter()
cerr = collections.Counter()
rfail = []
for r in tests:
    src = os.path.abspath(os.path.join("test/std", r["file"]))
    drv = "/tmp/rf_drv.cpp"
    open(drv, "w").write(f'#include "{src}"\nint main(){{ return {r["entry"]}; }}\n')
    exe = "/tmp/rf_exe"
    p = subprocess.run(CIS + [drv] + LINK + ["-o", exe],
                       capture_output=True, text=True, timeout=180)
    if p.returncode != 0:
        buckets["compile-fail"] += 1
        m = re.search(r"error: (.+)", p.stderr)
        key = re.sub(r"'[^']*'", "'X'", m.group(1))[:62] if m else "?"
        cerr[key] += 1
        continue
    try:
        rc = subprocess.run([exe], capture_output=True, timeout=60).returncode
    except subprocess.TimeoutExpired:
        rc = -99
    if rc == 0:
        buckets["PASS"] += 1
    else:
        buckets["run-fail"] += 1
        rfail.append(r["file"])

n = len(tests)
print(f"{pre}: {n} run-tests individually")
for k in ("PASS", "compile-fail", "run-fail"):
    print(f"  {k:13s} {buckets[k]:4d}  ({100*buckets[k]//max(1,n)}%)")
print("  top compile-error causes:")
for e, c in cerr.most_common(8):
    print(f"    {c:3d}  {e}")
if rfail:
    print("  run-fail files:")
    for f in rfail[:10]:
        print("    ", f)
