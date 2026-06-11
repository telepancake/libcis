#!/usr/bin/env python3
"""Consolidated-test stage of the build graph, generated from
test/std/manifest.json.

  --ninja            emit build/groups.ninja: per source directory one group
                     TU edge (deps: the member rec.jsons + manifest + this
                     tool), then per backend compile/link + run edges with
                     real .result outputs, and a board edge summarizing them.
  --emit KEY OUT     write ONE group TU (write-if-changed; the edge is
                     restat=1 so an unchanged TU prunes recompiles).

One TU per directory: the std headers are parsed once per ~7 tests instead of
per test (measured ~7-11x).  transfer.py already isolated each file in its own
namespace, so bodies concatenate verbatim; #includes are hoisted and deduped;
the synthesized main calls each run-test's recorded qualified entry.

All backends compile at gnu++20: the tests were adapted under TEST_STD_VER=20,
so a vendor run at a newer std would activate >=23 arms the transfer never saw
and report failures that say nothing about libcis.
"""
import hashlib
import json
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.chdir(ROOT)

MANIFEST = "test/std/manifest.json"
SRC_DIR = "build/groups/src"
RUN_TIMEOUT = 120  # seconds per test binary

BACKENDS = {
    "libcis":    ("g++-10",     "-std=gnu++20 -fno-exceptions -fno-rtti -nostdinc++ -Iinclude -Itest/std/support -O0 -w", True),
    "libcxx":    ("clang++-20", "-std=gnu++20 -stdlib=libc++ -fno-exceptions -fno-rtti -Itest/std/support -O0 -w", False),
    "libstdcxx": ("g++-14",     "-std=gnu++20 -fno-exceptions -fno-rtti -Itest/std/support -O0 -w", False),
    # discriminator: gcc-10 with its OWN libstdc++.  A test red here and red on
    # libcis is a gcc-10 limitation; red only on libcis is a libcis bug.
    "gcc10std":  ("g++-10",     "-std=gnu++20 -fno-exceptions -fno-rtti -Itest/std/support -O0 -w", False),
}
LINK_CIS = "-nodefaultlibs -lpthread -lm -lc -lgcc_s -lgcc"


def gkey(d):
    # injective: sibling dirs op++ / op= / op* collide under plain sanitizing
    return re.sub(r"[^0-9A-Za-z]+", "_", d) + "_" + hashlib.sha1(d.encode()).hexdigest()[:8]


def load_groups():
    man = json.load(open(MANIFEST))
    seen, groups = set(), {}
    for r in man["transferred"]:
        if r["file"] in seen:
            continue
        seen.add(r["file"])
        d = os.path.dirname(r["file"])
        groups.setdefault(d, []).append(r)
    return groups


def group_source(members):
    inc, seen, bodies, calls = [], set(), [], []
    for r in sorted(members, key=lambda r: r["file"]):
        body = []
        for ln in open(os.path.join("test/std", r["file"]), errors="replace").read().splitlines(True):
            if re.match(r"\s*#\s*include", ln):
                if ln.strip() not in seen:
                    seen.add(ln.strip())
                    inc.append(ln if ln.endswith("\n") else ln + "\n")
            else:
                body.append(ln)
        bodies.append("".join(body) + "\n")
        if r["kind"] == "run" and r.get("entry"):
            calls.append(f"  {r['entry']};")
    return ("".join(inc) + "\n" + "".join(bodies)
            + "\nint main(){\n" + "\n".join(calls) + "\n  return 0;\n}\n")


def write_if_changed(path, text):
    if os.path.exists(path) and open(path, errors="replace").read() == text:
        return False
    os.makedirs(os.path.dirname(path), exist_ok=True)
    open(path, "w").write(text)
    return True


def emit_one(key, out):
    for d, members in load_groups().items():
        if gkey(d) == key:
            write_if_changed(out, group_source(members))
            return
    sys.exit(f"no group {key} in {MANIFEST}")


def emit_ninja():
    groups = load_groups()
    flags = {d: sorted(set(f for r in ms for f in r.get("flags", [])))
             for d, ms in groups.items()}
    L = ["ninja_required_version = 1.10",
         "rule gengroup",
         "  command = python3 tools/gen_groups.py --emit $key $out",
         "  description = GROUP $out",
         "  restat = 1",
         "rule run",
         f'  command = ( timeout {RUN_TIMEOUT} "$in" >$out 2>&1; echo exit=$$? >>$out )',
         "  description = RUN $in",
         ""]
    for be, (cxx, cargs, has_sup) in BACKENDS.items():
        L += [f"cxx_{be} = {cxx}", f"cargs_{be} = {cargs}",
              f"rule link_{be}",
              f"  command = $cxx_{be} $cargs_{be} $flags $in $extra_{be} -o $out -MMD -MF $out.d",
              "  depfile = $out.d", "  deps = gcc",
              f"  description = LINK[{be}] $out", ""]
    sup = "build/groups/libcis/__support.o"
    L += ["rule cco",
          f"  command = $cxx_libcis $cargs_libcis -c $in -o $out -MMD -MF $out.d",
          "  depfile = $out.d", "  deps = gcc",
          f"build {sup}: cco src/support.cpp",
          f"extra_libcis = {sup} {LINK_CIS}",
          "extra_libcxx =", "extra_libstdcxx = ", "extra_gcc10std =", ""]

    results = {be: [] for be in BACKENDS}
    for d in sorted(groups):
        key = gkey(d)
        src = f"{SRC_DIR}/{key}.cpp"
        recs = " ".join(f"build/recs/{r['file']}.rec.json" for r in groups[d])
        L += [f"build {src}: gengroup {recs} | tools/gen_groups.py {MANIFEST}",
              f"  key = {key}"]
        fl = " ".join(flags[d])
        for be in BACKENDS:
            dep = f" | {sup}" if be == "libcis" else ""
            exe, res = f"build/groups/{be}/{key}.exe", f"build/groups/{be}/{key}.result"
            L.append(f"build {exe}: link_{be} {src}{dep}")
            if fl:
                L.append(f"  flags = {fl}")
            L.append(f"build {res}: run {exe}")
            results[be].append(res)
    # a red group = failed link edge = missing .result; run with -k0 and let
    # tools/board.py count the holes -- no edge may swallow a failure.
    for be in BACKENDS:
        L.append(f"build {be}: phony " + " $\n    ".join(results[be]))
    L += ["default libcis", ""]
    write_if_changed("build/groups.ninja", "\n".join(L))
    print(f"build/groups.ninja: {len(groups)} groups x {len(BACKENDS)} backends")


if __name__ == "__main__":
    if len(sys.argv) >= 2 and sys.argv[1] == "--ninja":
        emit_ninja()
    elif len(sys.argv) >= 4 and sys.argv[1] == "--emit":
        emit_one(sys.argv[2], sys.argv[3])
    else:
        sys.exit("usage: gen_groups.py --ninja | --emit KEY OUT")
