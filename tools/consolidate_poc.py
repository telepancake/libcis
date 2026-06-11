#!/usr/bin/env python3
# POC: consolidate a directory of already-transferred tests into ONE group TU.
# Hoist #include lines (preprocessor, not C++ syntax) to the top, wrap each
# file's body in an outer `namespace t_<n> {}` so its inner `using namespace`
# stays contained, and synthesize a main that calls each t_<n>::test_*().
import sys, os, re, glob, json
d = sys.argv[1]
# entry-point name per file is the slug the AST tool recorded (test_<slug>),
# not something to be guessed by regex.
man = json.load(open("test/std/manifest.json"))
slug = {r["file"]: r["slug"] for r in man["transferred"]}
kind = {r["file"]: r["kind"] for r in man["transferred"]}
files = sorted(glob.glob(d + "/**/*.pass.cpp", recursive=True))
inc, incset, bodies, calls = [], set(), [], []
for i, f in enumerate(files):
    rel = os.path.relpath(f, "test/std")
    if rel not in slug:
        continue
    txt = open(f).read()
    body = []
    for ln in txt.splitlines(keepends=True):
        if re.match(r'\s*#\s*include', ln):
            k = ln.strip()
            if k not in incset:
                incset.add(k); inc.append(ln if ln.endswith("\n") else ln + "\n")
        else:
            body.append(ln)
    ns = f"t_{i}"
    bodies.append(f"namespace {ns} {{\n{''.join(body)}\n}}\n")
    if kind[rel] == "run":
        calls.append(f"  {ns}::test_{slug[rel]}();")
out = "".join(inc) + "\n" + "".join(bodies) + \
      "\nint main() {\n" + "\n".join(calls) + "\n  return 0;\n}\n"
os.makedirs("build", exist_ok=True)
open("build/group.cpp", "w").write(out)
print(f"{len(files)} files -> build/group.cpp  ({len(calls)} run tests, "
      f"{len(inc)} unique includes, {len(out.splitlines())} lines)")
