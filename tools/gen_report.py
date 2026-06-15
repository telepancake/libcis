#!/usr/bin/env python3
"""gen_report.py - render test/std/REPORT.md from the measured results.json.

Pure aggregation of REAL measured counts (tools/measure_std.py).  No sampling,
no extrapolation: every number is a count of transferred files actually compiled
and/or run against the two vendor toolchains.
"""
import json
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
import config as cfg  # noqa: E402
DST = os.path.join(ROOT, "test", "std")


def pct(a, b):
    return f"{100.0*a/b:.0f}%" if b else "-"


def main():
    with open(os.path.join(DST, "manifest.json")) as fh:
        man = json.load(fh)
    with open(os.path.join(DST, "results.json")) as fh:
        res = json.load(fh)

    total_inputs = sum(len(man[k]) for k in ("transferred", "skipped", "errors"))
    total_xfer = len(man["transferred"])
    total_skip = len(man["skipped"])
    total_err = len(man["errors"])

    lines = []
    lines.append("# libcis test-transfer: Phase-1 vendor measurement\n")
    lines.append(
        "AST-transferred libc++ test suite (`tools/transfer.py`) measured against "
        "the two reference vendor toolchains. **No libcis numbers here** "
        "(Phase 2). Every count below is a real compile/run of a transferred "
        "file -- nothing sampled or extrapolated.\n")
    lines.append("- Transfer: `tools/transfer.py` (libclang AST rewrite)")
    lines.append("- Measure : `tools/measure_std.py` -> `results.json`")
    lines.append(f"- libcxx  : `{cfg.CXX_LIBCXX} -std=gnu++2c -stdlib=libc++`")
    lines.append(f"- libstdcxx: `{cfg.CXX_LIBSTDCXX} -std=gnu++26`")
    lines.append("- Both    : `-fno-exceptions -fno-rtti -I test/std/support`\n")

    lines.append("## Totals (transfer stage)\n")
    lines.append(f"- inputs found   : **{total_inputs}**")
    lines.append(f"- transferred    : **{total_xfer}** "
                 f"({pct(total_xfer, total_inputs)} of inputs)")
    lines.append(f"- skipped        : {total_skip} (UNSUPPORTED intersects our "
                 f"active no-exceptions/no-rtti, or REQUIRES unmet)")
    lines.append(f"- parse/xform err: {total_err}\n")

    # aggregate measured vendor totals over subtrees actually measured
    agg = {v: {"cp": 0, "ct": 0, "rp": 0, "rt": 0} for v in ("libcxx", "libstdcxx")}
    measured_xfer = 0
    for sub, r in res.items():
        measured_xfer += r.get("transferred", 0)
        for v in ("libcxx", "libstdcxx"):
            if v in r:
                d = r[v]
                agg[v]["cp"] += d["compile_pass"]
                agg[v]["ct"] += d["compile_pass"] + d["compile_fail"]
                agg[v]["rp"] += d["run_pass"]
                agg[v]["rt"] += d["run_pass"] + d["run_fail"]

    lines.append("## Measured vendor totals (over measured subtrees)\n")
    lines.append(f"Subtrees measured cover **{measured_xfer}** transferred tests.\n")
    lines.append("| vendor | compile-pass | run-pass |")
    lines.append("|---|---|---|")
    for v in ("libcxx", "libstdcxx"):
        a = agg[v]
        lines.append(f"| {v} | {a['cp']}/{a['ct']} ({pct(a['cp'], a['ct'])}) | "
                     f"{a['rp']}/{a['rt']} ({pct(a['rp'], a['rt'])}) |")
    lines.append("")

    lines.append("## Per-subtree breakdown\n")
    lines.append("`xfer` = transferred / inputs. compile & run are pass/total. "
                 "Run total counts only run-kind tests; compile-only "
                 "(`*.compile.pass.cpp`) tests have no run column entry.\n")
    lines.append("| subtree | inputs | xfer | skip | err | "
                 "cxx compile | cxx run | stdcxx compile | stdcxx run |")
    lines.append("|---|--:|--:|--:|--:|--:|--:|--:|--:|")
    for sub in sorted(res):
        r = res[sub]
        inp = r.get("inputs", 0)
        xf = r.get("transferred", 0)
        cells = [sub, str(inp), f"{xf}", str(r.get("skipped", 0)),
                 str(r.get("errors", 0))]
        for v in ("libcxx", "libstdcxx"):
            if v in r:
                d = r[v]
                ct = d["compile_pass"] + d["compile_fail"]
                rt = d["run_pass"] + d["run_fail"]
                cells.append(f"{d['compile_pass']}/{ct}")
                cells.append(f"{d['run_pass']}/{rt}" if rt else "-")
            else:
                cells.append("-"); cells.append("-")
        lines.append("| " + " | ".join(cells) + " |")
    lines.append("")

    with open(os.path.join(DST, "REPORT.md"), "w") as fh:
        fh.write("\n".join(lines) + "\n")
    print("wrote REPORT.md;", len(res), "subtrees measured")


if __name__ == "__main__":
    main()
