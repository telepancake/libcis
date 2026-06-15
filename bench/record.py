#!/usr/bin/env python3
"""record.py - measure libcis code size and append one parsable journal entry.

This is the entry point for the "batch of agents trying stuff" workflow: each
agent, after a change, runs

    bench/record.py "what I changed / am measuring"

which builds every bench project (see codesize.py), measures .text/.rodata/
.data/.bss, and appends ONE JSON object as a single line to bench/sizes.jsonl.

The description is mandatory: it is how an experiment is found later. Git state
(commit, dirty flag, branch) is captured automatically so a journal line can be
traced back to exactly what was measured. The append is done under an exclusive
file lock with O_APPEND, so many agents can record concurrently without
corrupting the log.

JSONL (one self-contained object per line) is chosen so the later analysis pass
can stream the file and pick out the heads/tails (smallest/largest .text per
project, deltas between commits, etc.) without a parser of its own.

Each line looks like:

  {"ts": "2026-06-15T05:54:00Z", "desc": "...", "commit": "ab0123d",
   "commit_full": "...", "dirty": false, "branch": "...", "cxx": "g++-10",
   "cxx_version": "g++-10 ... 10.5.0", "flags": "...", "ok": true,
   "baseline_text": 80633,
   "projects": {"fmt": {"text": 132111, "marginal": 51478, "rodata": 8097,
                        "data": 24, "bss": 5328, "ok": true}, ...}}

Usage:
    bench/record.py "<description>" [project ...]
    bench/record.py --print              # dump the journal as a table
"""
import argparse
import datetime
import fcntl
import json
import os
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import codesize  # noqa: E402

JOURNAL = os.path.join(codesize.ROOT, "bench", "sizes.jsonl")


def git(*args):
    try:
        return subprocess.run(["git", "-C", codesize.ROOT, *args],
                              stdout=subprocess.PIPE, stderr=subprocess.DEVNULL,
                              text=True).stdout.strip()
    except Exception:
        return ""


def git_state():
    dirty = bool(git("status", "--porcelain"))
    return {
        "commit": git("rev-parse", "--short", "HEAD"),
        "commit_full": git("rev-parse", "HEAD"),
        "dirty": dirty,
        "branch": git("rev-parse", "--abbrev-ref", "HEAD"),
    }


def cxx_version():
    try:
        out = subprocess.run(codesize.CXX + ["--version"],
                             stdout=subprocess.PIPE, text=True).stdout
        return out.splitlines()[0] if out else ""
    except Exception:
        return ""


def build_entry(desc, names):
    results = codesize.measure(names)
    base = next((r["text"] for r in results
                 if r.get("name") == "baseline" and r["ok"]), 0)
    projects = {}
    for r in results:
        if r["name"] == "baseline":
            continue
        if r["ok"]:
            projects[r["name"]] = {
                "text": r["text"], "marginal": r["text"] - base,
                "rodata": r["rodata"], "data": r["data"], "bss": r["bss"],
                "ok": True,
            }
        else:
            projects[r["name"]] = {"ok": False, "stage": r.get("stage", "?")}
    entry = {
        "ts": datetime.datetime.now(datetime.timezone.utc)
        .strftime("%Y-%m-%dT%H:%M:%SZ"),
        "desc": desc,
        **git_state(),
        "cxx": " ".join(codesize.CXX),
        "cxx_version": cxx_version(),
        "flags": " ".join(codesize.COMPILE),
        "ok": all(p.get("ok") for p in projects.values()),
        "baseline_text": base,
        "projects": projects,
    }
    return entry


def append_entry(entry):
    """Append one JSON line under an exclusive lock (concurrency-safe)."""
    line = json.dumps(entry, sort_keys=True) + "\n"
    fd = os.open(JOURNAL, os.O_WRONLY | os.O_CREAT | os.O_APPEND, 0o644)
    try:
        fcntl.flock(fd, fcntl.LOCK_EX)
        os.write(fd, line.encode("utf-8"))
    finally:
        fcntl.flock(fd, fcntl.LOCK_UN)
        os.close(fd)


def print_journal():
    if not os.path.exists(JOURNAL):
        print("(no journal yet)")
        return
    with open(JOURNAL) as fh:
        rows = [json.loads(ln) for ln in fh if ln.strip()]
    print(f"{'ts':<22}{'commit':<9}{'D':<2}{'ok':<3}{'desc'}")
    for r in rows:
        print(f"{r['ts']:<22}{r.get('commit', '?'):<9}"
              f"{'*' if r.get('dirty') else ' ':<2}"
              f"{'Y' if r.get('ok') else 'N':<3}{r.get('desc', '')}")


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("description", nargs="?",
                    help="REQUIRED: what is being measured (free text)")
    ap.add_argument("projects", nargs="*", help="subset (default: all)")
    ap.add_argument("--print", action="store_true", dest="print_only",
                    help="print the journal index and exit")
    args = ap.parse_args()

    if args.print_only:
        print_journal()
        return 0

    if not args.description:
        ap.error("a description is required (what is being measured)")
    if not codesize.have_compiler():
        print(f"compiler not found: {codesize.CXX[0]} (set $CXX to override)",
              file=sys.stderr)
        return 2

    entry = build_entry(args.description, args.projects or None)
    append_entry(entry)

    sizes = " ".join(f"{n}={p['text']}" for n, p in entry["projects"].items()
                     if p.get("ok"))
    dirty = " (dirty)" if entry["dirty"] else ""
    print(f"recorded @ {entry['commit']}{dirty}: {args.description}")
    print(f"  baseline={entry['baseline_text']} {sizes}")
    if not entry["ok"]:
        bad = [n for n, p in entry["projects"].items() if not p.get("ok")]
        print(f"  FAILED: {', '.join(bad)}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
