#!/usr/bin/env python3
"""Conformance board for one backend: tools/board.py [backend]

Reads test/std/manifest.json for the full group universe and
build/groups/<backend>/<key>.result for outcomes.  A group is green iff its
.result exists AND records exit=0; a missing .result (failed compile/link, or
never built) is RED -- the denominator is every group in the manifest, never
just the ones that happened to build.
"""
import collections
import json
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from gen_groups import gkey, load_groups  # noqa: E402

be = sys.argv[1] if len(sys.argv) > 1 else "libcis"
groups = load_groups()

rows = collections.defaultdict(lambda: [0, 0])  # subtree -> [green, total]
red = []
for d in sorted(groups):
    sub = d.split("/")[0]
    rows[sub][1] += 1
    res = f"build/groups/{be}/{gkey(d)}.result"
    ok = False
    if os.path.exists(res):
        txt = open(res, errors="replace").read()
        ok = txt.rstrip().endswith("exit=0")
    if ok:
        rows[sub][0] += 1
    else:
        red.append(d)

ntest = sum(len(ms) for ms in groups.values())
print(f"backend={be}  ({ntest} tests in {len(groups)} groups)")
print(f"{'subtree':<18} green/total")
g = t = 0
for sub in sorted(rows):
    a, b = rows[sub]
    g += a; t += b
    print(f"{sub:<18} {a:>5}/{b}")
print(f"{'TOTAL':<18} {g:>5}/{t}")
if "-v" in sys.argv:
    for d in red:
        print("RED", d)
