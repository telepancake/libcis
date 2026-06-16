#!/usr/bin/env python3
"""Batched transfer edge: one ninja edge per source DIRECTORY (the same
granularity the group TUs consume), so one Python+libclang+PCH startup serves
~7 files instead of 1 (~3-4x on a full pass).

  transfer_batch.py --edge-dir DIR_LABEL SRC...

The edge DECLARES every build/recs/<rel>.rec.json it writes as a ninja output
(a multi-output edge), so downstream group edges that depend on individual
recs resolve.  Crash isolation with attribution: the batch runs in ONE child;
if it dies or hangs, the parent redoes the directory's missing recs per-file
(transfer_batch falls back to transfer.cmd_edge, which always writes a rec),
so every declared output exists even after a segfault.
"""
import json
import os
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import transfer as T  # noqa: E402

BATCH_TIMEOUT = 600  # whole-directory budget; per-file fallback has its own


def rec_path(rel):
    return os.path.join(T.ROOT, "build", "recs", rel + ".rec.json")


def run_batch_child(pairs):
    """Child: process files sequentially, write each rec immediately (so a
    crash loses only the file it crashed on), echo progress on stdout."""
    # Bind libclang once for this child (mirrors cmd_worker/cmd_edge). Without
    # this, idx.parse silently fails with TranslationUnitLoadError because the
    # cindex Config has no library path set.
    T._ensure_libclang()
    for src, rel in pairs:
        print(rel, flush=True)
        T.write_rec(rec_path(rel), T.process_one((src, rel)))


def main():
    if sys.argv[1] == "--batch-child":
        pairs = json.load(open(sys.argv[2]))
        run_batch_child(pairs)
        return
    assert sys.argv[1] == "--edge-dir"
    label, srcs = sys.argv[2], sys.argv[3:]
    pairs = [(s, os.path.relpath(os.path.abspath(s), T.SRC_STD)) for s in srcs]

    plan = os.path.join(T.ROOT, "build", "recs", ".plan." + label + ".json")
    os.makedirs(os.path.dirname(plan), exist_ok=True)
    json.dump(pairs, open(plan, "w"))
    try:
        p = subprocess.run(
            [sys.executable, __file__, "--batch-child", plan],
            capture_output=True, text=True, timeout=BATCH_TIMEOUT)
        crashed = p.returncode != 0
    except subprocess.TimeoutExpired as ex:
        p, crashed = ex, True
    if crashed:
        # batch died mid-directory; redo every file lacking a rec via the
        # per-file isolated path (cmd_edge attributes crash/timeout to the
        # file itself and always writes a rec).
        for src, rel in pairs:
            if not os.path.exists(rec_path(rel)):
                T.cmd_edge(src, rel, rec_path(rel))
    os.unlink(plan)
    # every declared output must now exist
    for _, rel in pairs:
        if not os.path.exists(rec_path(rel)):
            T.cmd_edge(*[s for s in pairs if s[1] == rel][0], rec_path(rel))


if __name__ == "__main__":
    main()

