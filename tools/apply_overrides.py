#!/usr/bin/env python3
"""Re-apply tools/test_overrides/ onto test/std/.

tools/test_overrides/ holds hand-adapted copies of transferred tests whose
upstream form trips PROVEN gcc-10 frontend defects (evidence in each file's
"libcis:" comments).  The transfer reproduces the unmodified upstream form, so
these adaptations must be re-applied on top of it.  test/std/ is committed, so
`make transfer` runs this as its final step to bake the overrides into the
committed tree; run_files.py also calls it so the per-file gate is self-healing
regardless of transfer order.  Idempotent: a file already matching its override
is left untouched (so a clean tree stays clean).

The override files hardcode `libcis_ns_<slug>`, but slug_for() hashes the path
into the slug, so an override's namespace is rewritten to the slug the manifest
actually recorded for that path -- otherwise the group/gate driver calls
`libcis_ns_<hashed>::main()` while the override declares `libcis_ns_<old>`.
"""
import json
import os
import re

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OVR = os.path.join(ROOT, "tools", "test_overrides")
DST_ROOT = os.path.join(ROOT, "test", "std")
MANIFEST = os.path.join(DST_ROOT, "manifest.json")


def apply_overrides():
    """Copy every tools/test_overrides/<rel> onto test/std/<rel>, rewriting its
    namespace to the manifest's slug.  Returns the number of files changed."""
    slug = {}
    if os.path.exists(MANIFEST):
        slug = {r["file"]: r["slug"]
                for r in json.load(open(MANIFEST))["transferred"] if r.get("slug")}
    changed = 0
    for dirp, _, files in os.walk(OVR):
        for fn in files:
            src = os.path.join(dirp, fn)
            rel = os.path.relpath(src, OVR)
            dst = os.path.join(DST_ROOT, rel)
            text = open(src).read()
            s = slug.get(rel.replace(os.sep, "/"))
            if s:
                text = re.sub(r"libcis_ns_\w+", "libcis_ns_" + s, text)
            if os.path.exists(dst) and open(dst).read() != text:
                open(dst, "w").write(text)
                changed += 1
    return changed


if __name__ == "__main__":
    n = apply_overrides()
    print(f"apply_overrides: {n} file(s) updated under test/std/")
