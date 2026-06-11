#!/usr/bin/env python3
"""Batched transfer edge: one ninja edge per source DIRECTORY (the same
granularity the group TUs consume), so one Python+libclang+PCH startup serves
~7 files instead of 1 (~3-4x on a full pass).

  transfer_batch.py --edge-dir DIR_REL OUT_DIRREC SRC...

Writes build/recs/<rel>.rec.json per file (same records as transfer.py
--worker) plus OUT_DIRREC as the edge's declared output.  Crash isolation is
preserved with attribution: the batch runs in ONE child process; if the child
dies or hangs, the parent falls back to per-file children for exactly this
directory, so a segfaulting input costs its file, not the directory.
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
    for src, rel in pairs:
        print(rel, flush=True)
        T.write_rec(rec_path(rel), T.process_one((src, rel)))


def main():
    if sys.argv[1] == "--batch-child":
        pairs = json.load(open(sys.argv[2]))
        run_batch_child(pairs)
        return
    assert sys.argv[1] == "--edge-dir"
    dirrec, srcs = sys.argv[2], sys.argv[3:]
    pairs = [(s, os.path.relpath(os.path.abspath(s), T.SRC_STD)) for s in srcs]

    plan = os.path.join(T.ROOT, "build", "recs", ".plan." +
                        os.path.basename(dirrec) + ".json")
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
        # the batch died mid-directory; the echoed progress names the last
        # file it started.  Redo every file lacking a rec via the per-file
        # isolated path (which attributes crash/timeout to the file itself).
        done = set((p.stdout or "").split()[:-1])  # last name = the culprit-or-victim
        for src, rel in pairs:
            if rel not in done or not os.path.exists(rec_path(rel)):
                T.cmd_edge(src, rel, rec_path(rel))
    os.unlink(plan)

    # the directory record is the edge's declared output: it lists the per-file
    # recs and changes iff any of them changed (content hash), so downstream
    # (manifest) rebuilds exactly when results moved.
    import hashlib
    h = hashlib.sha1()
    for _, rel in sorted(pairs):
        rp = rec_path(rel)
        h.update(rel.encode())
        h.update(open(rp, "rb").read() if os.path.exists(rp) else b"?")
    os.makedirs(os.path.dirname(dirrec), exist_ok=True)
    with open(dirrec, "w") as fh:
        json.dump({"files": [rel for _, rel in pairs],
                   "digest": h.hexdigest()}, fh)


if __name__ == "__main__":
    main()
