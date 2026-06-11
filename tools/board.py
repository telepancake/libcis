#!/usr/bin/env python3
"""Conformance board for one backend: tools/board.py [backend] [-v]

Two granularities, both honest:
  GROUPS  - a group is green iff its .result ends exit=0 (every test passed);
            the denominator is every group in the manifest (missing .result =
            red), never just the ones that built.
  TESTS   - from the fork-per-test harness: a completed group prints
            'LIBCIS-DONE total=T fails=F' so green tests = T-F.  A group whose
            .result lacks LIBCIS-DONE crashed/hung/failed-to-compile before
            finishing -> all its run-tests (from the manifest) count red.
"""
import collections
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from gen_groups import gkey, load_groups  # noqa: E402

be = sys.argv[1] if len(sys.argv) > 1 else "libcis"
groups = load_groups()

grp = collections.defaultdict(lambda: [0, 0])   # subtree -> [green, total]
tst = collections.defaultdict(lambda: [0, 0])   # subtree -> [green, total]
red, incomplete = [], []
for d in sorted(groups):
    sub = d.split("/")[0]
    runtests = sum(1 for r in groups[d] if r["kind"] == "run")
    grp[sub][1] += 1
    tst[sub][1] += runtests
    res = f"build/groups/{be}/{gkey(d)}.result"
    txt = open(res, errors="replace").read() if os.path.exists(res) else ""
    if txt.rstrip().endswith("exit=0"):
        grp[sub][0] += 1
    else:
        red.append(d)
    m = re.search(r"LIBCIS-DONE total=(\d+) fails=(\d+)", txt)
    if m:
        total, fails = int(m.group(1)), int(m.group(2))
        tst[sub][0] += max(0, total - fails)
    else:
        incomplete.append(d)   # never finished -> 0 green tests credited

ntest = sum(len(ms) for ms in groups.values())
print(f"backend={be}  ({ntest} tests in {len(groups)} groups)")
gg = gt = tg = tt = 0
print(f"{'subtree':<18} {'groups':>9}  {'tests':>11}")
for sub in sorted(grp):
    a, b = grp[sub]; c, d2 = tst[sub]
    gg += a; gt += b; tg += c; tt += d2
    print(f"{sub:<18} {a:>4}/{b:<4}  {c:>5}/{d2}")
print(f"{'TOTAL':<18} {gg:>4}/{gt:<4}  {tg:>5}/{tt}")
print(f"incomplete groups (crash/hang/compile-fail, no per-test signal): {len(incomplete)}")
if "-v" in sys.argv:
    for d in red:
        print("RED", d)
