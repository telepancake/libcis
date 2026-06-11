#!/usr/bin/env python3
# Consolidate the transferred libc++ suite into ONE translation unit per source
# directory, and emit a ninja build that compiles+runs each group.
#   - each file's body goes in `namespace t<n> {}` (its `using namespace` stays
#     contained); #include lines are hoisted+deduped to the top of the group.
#   - a synthesized main calls each run-kind test.
#   - -MMD depfiles -> editing a library header rebuilds only affected groups.
#   - target of a run is a real <group>.result file (captured output + exit=N),
#     not a stamp.
# Usage: tools/gen_groups.py <backend> [subtree ...]
import json, os, re, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.chdir(ROOT)

BACKENDS = {
    "libcis":    ("g++-10",     "-std=gnu++20 -fno-exceptions -fno-rtti -nostdinc++ -Iinclude -Itest/std/support -O0 -w", True),
    "libcxx":    ("clang++-20", "-std=gnu++2c -stdlib=libc++ -fno-exceptions -fno-rtti -Itest/std/support -O0 -w", False),
    "libstdcxx": ("g++-14",     "-std=gnu++26 -fno-exceptions -fno-rtti -Itest/std/support -O0 -w", False),
}
LINK_CIS = "-nodefaultlibs -lpthread -lm -lc -lgcc_s -lgcc"

def group_source(files):
    inc, seen, bodies, calls = [], set(), [], []
    for i, (rel, slug, kind) in enumerate(files):
        body = []
        for ln in open(os.path.join("test/std", rel), errors="replace").read().splitlines(True):
            if re.match(r"\s*#\s*include", ln):
                if ln.strip() not in seen:
                    seen.add(ln.strip()); inc.append(ln if ln.endswith("\n") else ln + "\n")
            else:
                body.append(ln)
        bodies.append(f"namespace t{i} {{\n{''.join(body)}\n}}\n")
        if kind == "run":
            calls.append(f"  t{i}::test_{slug}();")
    return "".join(inc) + "\n" + "".join(bodies) + "\nint main(){\n" + "\n".join(calls) + "\n  return 0;\n}\n"

def gen(backend, subtrees):
    cxx, cargs, has_sup = BACKENDS[backend]
    man = json.load(open("test/std/manifest.json"))
    seen, tests = set(), []
    for r in man["transferred"]:
        if r["file"] in seen:
            continue
        seen.add(r["file"]); tests.append(r)
    if subtrees:
        tests = [r for r in tests if any(r["file"].split("/")[0] == s for s in subtrees)]

    groups = {}
    flags = {}
    for r in tests:
        d = os.path.dirname(r["file"])
        groups.setdefault(d, []).append((r["file"], r["slug"], r["kind"]))
        flags.setdefault(d, set()).update(r.get("flags", []))

    gdir = f"build/groups/{backend}"
    os.makedirs(gdir, exist_ok=True)
    for d, files in groups.items():
        src = f"{gdir}/" + re.sub(r"[^0-9A-Za-z]+", "_", d) + ".cpp"
        open(src, "w").write(group_source(sorted(files)))

    L = ["ninja_required_version = 1.10", "builddir = build",
         f"cxx = {cxx}", f"cargs = {cargs}", ""]
    sup = ""
    if has_sup:
        sup = f"{gdir}/__support.o"
        L += ["rule cco", "  command = $cxx $cargs -c $in -o $out -MMD -MF $out.d",
              "  depfile = $out.d", "  deps = gcc",
              f"build {sup}: cco src/support.cpp", ""]
    link = (sup + " " + LINK_CIS) if has_sup else "-pthread"
    L += ["rule link",
          f"  command = $cxx $cargs $flags $in {link} -o $out -MMD -MF $out.d",
          "  depfile = $out.d", "  deps = gcc", "  description = LINK $out",
          "rule run",
          '  command = ( "$in" >$out 2>&1; echo exit=$$? >>$out )',
          "  description = RUN $in", ""]
    stamps = []
    for d, files in sorted(groups.items()):
        key = re.sub(r"[^0-9A-Za-z]+", "_", d)
        src, exe, res = f"{gdir}/{key}.cpp", f"{gdir}/{key}.exe", f"{gdir}/{key}.result"
        order = f" | {sup}" if sup else ""
        L.append(f"build {exe}: link {src}{order}")
        if flags[d]:
            L.append("  flags = " + " ".join(sorted(flags[d])))
        L.append(f"build {res}: run {exe}")
        stamps.append(res)
    L.append("build all: phony $\n    " + " $\n    ".join(stamps))
    L += ["default all", ""]
    out = f"build/groups.{backend}.ninja"
    open(out, "w").write("\n".join(L))
    print(f"{out}: {len(tests)} tests in {len(groups)} directory-groups")

if __name__ == "__main__":
    be = sys.argv[1] if len(sys.argv) > 1 else "libcis"
    gen(be, sys.argv[2:])
