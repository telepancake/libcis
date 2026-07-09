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
check(runs >= 5000, f"only {runs} run-kind tests (copy regression?)")

# 3. LOUD, non-fatal notes -- tests that silently left the verdict's
# denominator.  The build goes on, but each one is a test with NO verdict, so
# they must be impossible to miss in the build log.
WARN = []

# a .pass.cpp whose entry point was not found is recorded kind="compile": it
# still compiles in its group but never RUNS -- a silent run->compile downgrade.
downgraded = [r["file"] for r in man["transferred"]
              if r["kind"] == "compile"
              and not r["file"].endswith(".compile.pass.cpp")]
if downgraded:
    WARN.append(f"{len(downgraded)} run tests downgraded to compile-only "
                "(no entry recorded -- they will never RUN):")
    WARN += [f"    {f}" for f in downgraded[:20]]
    if len(downgraded) > 20:
        WARN.append(f"    ... and {len(downgraded) - 20} more")

# transfer errors (libclang crash/hang/parse failure): recorded in the
# manifest but absent from every denominator downstream.
errors = man.get("errors", [])
if errors:
    stages = {}
    for e in errors:
        stages[e["stage"]] = stages.get(e["stage"], 0) + 1
    WARN.append(f"{len(errors)} transfer errors (these tests are in NO "
                "denominator): "
                + " ".join(f"{s}={n}" for s, n in sorted(stages.items())))
    WARN += [f"    [{e['stage']}] {e['file']}" for e in errors[:20]]
    if len(errors) > 20:
        WARN.append(f"    ... and {len(errors) - 20} more")

if WARN:
    print(f"TRIPWIRE WARN (build continues; {len(downgraded) + len(errors)} "
          "tests have NO verdict):")
    for m in WARN:
        print("  !", m)

if FAIL:
    print("TRIPWIRE FAILED:")
    for m in FAIL:
        print("  -", m)
    sys.exit(1)
open(sys.argv[1], "w").write("ok\n")
print(f"tripwire: ok ({runs} run tests, adaptation live, "
      f"skipped={len(man.get('skipped', []))} errors={len(errors)} "
      f"downgraded={len(downgraded)})")
