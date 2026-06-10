#!/usr/bin/env python3
"""Generate a ninja build file for the AST-transferred libc++ test suite.

For each transferred test (test/std/manifest.json):
  - run-kind: compile the test source to a .o WITH A DEPFILE (-MMD), compile the
    generic driver to a .o (-DTESTFN=test_<slug>), link them (+ libcis's
    src/support.o for the libcis backend) into an exe, then run it -> .pass stamp.
  - compile-kind (*.compile.pass.cpp): -fsyntax-only WITH A DEPFILE -> .pass stamp.

Each source's depfile makes ninja track EVERY header it reads -- including
libcis's include/* -- so editing a library header rebuilds + reruns ONLY the
dependent tests. src/support.cpp is compiled once.

Usage:
  tools/gen_ninja.py [libcis|libcxx|libstdcxx|all]   (default: all)
  ninja -f build/<backend>.ninja [-k 0] [-j N] [<subtree>]
"""
import hashlib, json, os, re, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.chdir(ROOT)

BACKENDS = {
    "libcis": dict(
        cxx="g++-10",
        cargs="-std=gnu++20 -fno-exceptions -fno-rtti -nostdinc++ -Iinclude -Itest -Itest/std/support -O0 -g -w",
        link="-nodefaultlibs -lpthread -lm -lc -lgcc_s -lgcc",
        support=True),
    "libcxx": dict(
        cxx="clang++-20",
        cargs="-std=gnu++2c -stdlib=libc++ -fno-exceptions -fno-rtti -Itest/std/support -O0 -g -w",
        link="-pthread", support=False),
    "libstdcxx": dict(
        cxx="g++-14",
        cargs="-std=gnu++26 -fno-exceptions -fno-rtti -Itest/std/support -O0 -g -w",
        link="-pthread", support=False),
}

def phony(name, deps):
    if not deps:
        return f"build {name}: phony"
    out = [f"build {name}: phony $"]
    out += [f"    {d} $" for d in deps[:-1]]
    out.append(f"    {deps[-1]}")
    return "\n".join(out)

def gen(backend):
    b = BACKENDS[backend]
    man = json.load(open("test/std/manifest.json"))
    seen, tests = set(), []
    for t in man["transferred"]:           # manifest may have appended dups
        if t["file"] not in seen:
            seen.add(t["file"]); tests.append(t)
    bdir = f"build/ninja/{backend}"
    os.makedirs(bdir, exist_ok=True)

    L = [
        "ninja_required_version = 1.10",
        f"cxx = {b['cxx']}", f"cargs = {b['cargs']}", f"link = {b['link']}", "",
        "rule cc",        # compile one source -> .o (+ depfile: tracks all headers)
        "  command = $cxx $cargs $flags -c $in -o $out -MMD -MF $out.d",
        "  depfile = $out.d", "  deps = gcc", "  description = CC $out",
        "rule link",
        "  command = $cxx $cargs $in $sup $link -o $out",
        "  description = LINK $out",
        "rule run",
        "  command = $in && : > $out",
        "  description = RUN $in",
        "rule ccsyntax",  # compile-only test -> stamp (+ depfile)
        "  command = $cxx $cargs $flags -fsyntax-only $in -MMD -MF $out.d && : > $out",
        "  depfile = $out.d", "  deps = gcc", "  description = SYNTAX $out", "",
    ]
    sup = ""
    if b["support"]:
        sup = f"{bdir}/__support.o"
        L += [f"build {sup}: cc src/support.cpp", "  flags =", ""]

    by_sub = {}
    for t in tests:
        slug, kind, flags = t["slug"], t["kind"], " ".join(t.get("flags", []))
        src = "test/std/" + t["file"]
        h = hashlib.sha1(t["file"].encode()).hexdigest()[:8]
        base = f"{bdir}/" + re.sub(r"[^0-9A-Za-z]+", "_", t["file"]) + "_" + h
        stamp = base + ".pass"
        if kind == "run":
            to, do, exe = base + ".test.o", base + ".drv.o", base + ".exe"
            L.append(f"build {to}: cc {src}")
            L.append(f"  flags = {flags}")
            L.append(f"build {do}: cc tools/std_driver.cpp")
            L.append(f"  flags = -DTESTFN=test_{slug}")
            link_in = f"{to} {do}" + (f" | {sup}" if sup else "")
            L.append(f"build {exe}: link {link_in}")
            if sup:
                L.append(f"  sup = {sup}")
            L.append(f"build {stamp}: run {exe}")
        else:
            L.append(f"build {stamp}: ccsyntax {src}")
            L.append(f"  flags = {flags}")
        by_sub.setdefault(t["file"].split("/")[0], []).append(stamp)

    L.append("")
    for sub, stamps in sorted(by_sub.items()):
        L.append(phony(sub, stamps))
    L.append(phony("all", sorted(by_sub.keys())))
    L += ["default all", ""]

    open(f"build/{backend}.ninja", "w").write("\n".join(L))
    nr = sum(1 for t in tests if t["kind"] == "run")
    print(f"build/{backend}.ninja: {len(tests)} tests ({nr} run, {len(tests)-nr} compile-only), {len(by_sub)} subtrees")

want = sys.argv[1] if len(sys.argv) > 1 else "all"
for be in ([want] if want in BACKENDS else BACKENDS):
    gen(be)
