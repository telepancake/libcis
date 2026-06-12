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

# -fcoroutines: gcc-10 gates the C++20 co_* keywords behind it (later gccs do not)
CIS = ("g++-10 -std=gnu++20 -fcoroutines -fno-exceptions -fno-rtti -nostdinc++ -Iinclude "
       "-Itest/std -Itest/std/support -O0 -w").split()
LINK = "build/groups/libcis/libsupport.a -nodefaultlibs -lpthread -lm -lc -lgcc_s -lgcc".split()

# tools/test_overrides/ holds hand-adapted copies of transferred tests whose
# upstream form trips PROVEN gcc-10 frontend defects (evidence in the files'
# "libcis:" comments).  test/std is regenerated (gitignored), so the transfer
# would silently revert those adaptations; re-applying them here makes the
# gate self-healing regardless of transfer order.
OVR = os.path.join(ROOT, "tools", "test_overrides")
for dirp, _, files in os.walk(OVR):
    for fn in files:
        src = os.path.join(dirp, fn)
        dst = os.path.join(ROOT, "test/std", os.path.relpath(src, OVR))
        if os.path.exists(dst) and open(src).read() != open(dst).read():
            open(dst, "w").write(open(src).read())

pre = sys.argv[1]
limit = int(sys.argv[2]) if len(sys.argv) > 2 else 10**9
man = json.load(open("test/std/manifest.json"))

# Excluded tests carry a written justification (gcc-10 compiler limits proven by
# the discriminator, target-impossible facilities).  They are NOT the work queue
# and NOT counted against the verdict: every test NOT excluded must pass.
excl = {k: v for k, v in json.load(open("tools/exclusions.json")).items()
        if not k.startswith("_")}
tests = [r for r in man["transferred"]
         if r["file"].startswith(pre) and r["kind"] == "run" and r.get("entry")
         and r["file"] not in excl][:limit]

buckets = collections.Counter()
cerr = collections.Counter()
rfail = []
cfail = []
for r in tests:
    src = os.path.abspath(os.path.join("test/std", r["file"]))
    # PID-unique temp paths: concurrent run_files invocations (parallel
    # subtree gates) must not clobber each other's driver/executable.
    drv = f"/tmp/rf_drv_{os.getpid()}.cpp"
    open(drv, "w").write(f'#include "{src}"\nint main(){{ return {r["entry"]}; }}\n')
    exe = f"/tmp/rf_exe_{os.getpid()}"
    try:
        p = subprocess.run(CIS + [drv] + LINK + ["-o", exe],
                           capture_output=True, text=True, timeout=180)
    except subprocess.TimeoutExpired:
        buckets["compile-fail"] += 1
        cerr["compile-timeout"] += 1
        cfail.append((r["file"], "compile-timeout (180s)"))
        continue
    if p.returncode != 0:
        buckets["compile-fail"] += 1
        m = re.search(r"error: (.+)", p.stderr)
        key = re.sub(r"'[^']*'", "'X'", m.group(1))[:62] if m else "?"
        cerr[key] += 1
        cfail.append((r["file"], m.group(1)[:80] if m else "?"))
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

# State is binary: a subtree is CLEAN iff nothing in it fails.  We don't count
# worms -- we list them, so each can be fixed or moved (justified) into the
# exclusion set.  No pass-count, no ratio, no error-class census.
for f in rfail:
    print(f"FAIL(run)     {f}")
for f, e in cfail:
    print(f"FAIL(compile) {f}\n              {e}")
print("CLEAN" if not (rfail or cfail) else "NOT CLEAN")
sys.exit(0 if not (rfail or cfail) else 1)
