#!/usr/bin/env python3
"""Fixture assertions gating the pipeline: prove the AST stage is actually ON.

The failure mode this exists for: a regression that turns the transfer into a
plain copy still produces parseable files and green recs (an unmodified test
re-parses fine), so no other stage notices.  These known-input/known-output
checks fail the build instead.

Usage: tripwire.py OUT_STAMP   (reads test/std/manifest.json + outputs)
"""
import json
import sys

FAIL = []


def check(cond, msg):
    if not cond:
        FAIL.append(msg)


man = json.load(open("test/std/manifest.json"))
byfile = {r["file"]: r for r in man["transferred"]}

# 1. adaptation: type.pass.cpp has 4 unguarded typeid sites -> excised
f = "utilities/any/any.class/any.observers/type.pass.cpp"
r = byfile.get(f)
check(r is not None, f"{f}: not transferred")
if r:
    check(r["kind"] == "run", f"{f}: kind={r['kind']}, want run")
    check(r.get("adapted", {}).get("rtti", 0) >= 4,
          f"{f}: adapted={r.get('adapted')}, want rtti>=4")
    check(r.get("entry", "") and "::main(" in r["entry"],
          f"{f}: entry={r.get('entry')}")
    txt = open("test/std/" + f).read()
    check(txt.count("libcis: removed") >= 4, f"{f}: excision comments missing")
    # current wrapper: `namespace libcis_ns_<slug> { ... } // ...` (the file
    # body is namespaced; no `using namespace` re-export -- siblings must not
    # collide on consolidation).
    check("namespace libcis_ns_" in txt and "using namespace libcis_ns_" not in txt,
          f"{f}: not namespace-wrapped (or stale re-export present)")

# 2. suite-level: a copy-regression zeroes these
runs = sum(1 for r in man["transferred"] if r["kind"] == "run")
entries = sum(1 for r in man["transferred"] if r.get("entry"))
check(runs >= 5000, f"only {runs} run-kind tests (copy regression?)")
check(entries >= runs, f"{entries} entries < {runs} run tests")

if FAIL:
    print("TRIPWIRE FAILED:")
    for m in FAIL:
        print("  -", m)
    sys.exit(1)
open(sys.argv[1], "w").write("ok\n")
print(f"tripwire: ok ({runs} run tests, adaptation live)")
