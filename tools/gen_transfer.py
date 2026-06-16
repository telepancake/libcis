#!/usr/bin/env python3
"""Emit build/transfer.ninja + the root build/build.ninja: the libc++ ->
libcis test transfer as a real, incremental dependency graph.

Graph:
    tools/transfer.py, tools/transfer_batch.py  (implicit deps of the edges)
        build/transfer.pch                            (rule pch)
        build/recs/<dir>.dirrec.json : <dir srcs> | py pch   (rule xferdir)
            ... which writes build/recs/<rel>.rec.json per file
        test/std/manifest.json : <all dirrecs>        (rule aggregate, rspfile)
        build/tripwire.ok      : manifest             (rule tripwire)
        build/groups.ninja     : manifest, tripwire   (generator, then subninja)

One edge per source DIRECTORY (the granularity the group TUs consume): one
Python+libclang+PCH startup serves ~7 files instead of 1.  Crash isolation
keeps per-file attribution: the batch child writes each rec as it goes, and on
a crash/hang the parent redoes the directory's missing recs per-file under
individual timeouts (transfer_batch.py).

The tripwire (tools/tripwire.py) asserts known fixtures (adaptation excised
RTTI sites, entries recorded, suite-level run-test counts) so a regression
that degrades the AST stage to a copy FAILS the build instead of producing
green recs of unmodified files.

Usage: tools/gen_transfer.py [SUBTREE ...]   (default: every std/ subtree)
"""
import os
import re
import shutil
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import transfer as T  # noqa: E402

ROOT = T.ROOT
REC_DIR = os.path.join(ROOT, "build", "recs")


# Appended to the copied test_macros.h. Some support headers (any_helpers.h,
# atomic_helpers.h, ...) use libc++-INTERNAL _LIBCPP_AVAILABILITY_* macros
# unguarded; they only preprocess against libc++'s own <__config>. Every
# backend here that is not libc++ (libcis, libstdcxx, gcc10std) needs them
# defined away or the headers fail to PARSE ("expected initializer before").
SUPPORT_COMPAT = """
// --- libcis compatibility (appended by tools/gen_transfer.py) ---
// libc++-internal availability markers used unguarded by support headers;
// no-ops on every non-libc++ backend.
#ifndef _LIBCPP_AVAILABILITY_THROW_BAD_ANY_CAST
#  define _LIBCPP_AVAILABILITY_THROW_BAD_ANY_CAST
#endif
#ifndef _LIBCPP_AVAILABILITY_SYNC
#  define _LIBCPP_AVAILABILITY_SYNC
#endif
#ifndef _LIBCPP_AVAILABILITY_TZDB
#  define _LIBCPP_AVAILABILITY_TZDB
#endif
"""


# gcc-10 miscompiles DoNotOptimize's in-out barrier when `value` is an array:
# the `"+m,r"` operand decays the array to a pointer for the `r` alternative
# and the write-back clobbers the array's bytes (proven single-threaded: a
# 0xcd-filled stack array reads back as garbage after DoNotOptimize(arr)).  An
# input-only memory-clobber fence over the object's address is an equally strong
# barrier against dead-store elimination and is array-safe for every type.
DNO_BAD = '  asm volatile("" : "+m,r"(value) : : "memory");'
DNO_FIX = ('  { const volatile void* __cis_p = __builtin_addressof(value);\n'
           '    asm volatile("" : : "r"(__cis_p) : "memory"); }')


# The 18.x corpus's test_macros.h guards <version> with __has_include("<version>")
# -- a QUOTED operand, which gcc (and clang) treat as a literal filename and so
# always evaluate false, falling back to #include <ciso646>.  ciso646 was removed
# in C++20 and libcis (rightly) does not ship it, so the fallback fails to find a
# header.  Pin it to the unquoted form newer corpora use, which finds <version>.
CISO646_BAD = '__has_include("<version>")'
CISO646_FIX = '__has_include(<version>)'

# count_new.h defines 16 file-scope `operator new`/`operator delete` overloads,
# a `MemCounter& globalMemCounter = ...` global, and a `MemCounter::disable_checking`
# static-member definition -- all with plain external linkage. Per-test ELFs
# hide this; a single-ELF link of the whole suite hits ODR. The fix is
# linkage-only: turn every file-scope op-new/op-delete defn into an inline
# definition (one `inline` token prefix), make globalMemCounter a C++17 inline
# variable, and inline the static-member defn. getGlobalMemCounter() is already
# inline upstream so its function-local static is naturally shared across TUs.
CNEW_OPNEW_RE = re.compile(
    r'^(void\*? operator (?:new|delete)(?:\[\])?\([^)]*\)[^{;]*\{)',
    re.MULTILINE)
CNEW_DISABLE_RE = re.compile(
    r'^(\s*)(?!inline )(const bool MemCounter::disable_checking)',
    re.MULTILINE)
CNEW_GLOBAL_OLD = "MemCounter &globalMemCounter = *getGlobalMemCounter();"
CNEW_GLOBAL_NEW = "inline MemCounter &globalMemCounter = *getGlobalMemCounter();"


def _patch_count_new(path):
    """Prefix every file-scope op-new/op-delete defn with `inline`, inline the
    `MemCounter::disable_checking` static-member defn, and make
    globalMemCounter an inline variable. Idempotent."""
    cur = open(path).read()
    new = CNEW_OPNEW_RE.sub(lambda m: "inline " + m.group(1), cur)
    new = CNEW_DISABLE_RE.sub(r'\1inline \2', new)
    if CNEW_GLOBAL_NEW not in new:
        new = new.replace(CNEW_GLOBAL_OLD, CNEW_GLOBAL_NEW)
    if new != cur:
        open(path, "w").write(new)


def patch_support():
    """Idempotently append the compat block + array-safe DoNotOptimize fix +
    linkage fixes to count_new.h (inline op-new/op-delete + inline global)."""
    tm = os.path.join(T.DST_SUPPORT, "test_macros.h")
    if os.path.exists(tm):
        cur = open(tm).read()
        if DNO_BAD in cur or CISO646_BAD in cur:
            cur = cur.replace(DNO_BAD, DNO_FIX).replace(CISO646_BAD, CISO646_FIX)
            open(tm, "w").write(cur)
        if "libcis compatibility" not in cur:
            open(tm, "a").write(SUPPORT_COMPAT)
    cn = os.path.join(T.DST_SUPPORT, "count_new.h")
    if os.path.exists(cn):
        _patch_count_new(cn)


def copy_support(subtrees):
    """Static file copies (support harness + each subtree's sibling .h) done at
    generation time -- they are inputs, not transformed artifacts."""
    if not os.path.isdir(T.DST_SUPPORT):
        shutil.copytree(T.SRC_SUPPORT, T.DST_SUPPORT)
    patch_support()
    for sub in subtrees:
        T.copy_sibling_headers(sub)


def main():
    subtrees = sys.argv[1:] or sorted(
        d for d in os.listdir(T.SRC_STD) if os.path.isdir(os.path.join(T.SRC_STD, d)))
    os.makedirs(T.DST_ROOT, exist_ok=True)
    copy_support(subtrees)

    tooldir = os.path.relpath(os.path.dirname(os.path.abspath(__file__)), ROOT)
    py = f"{tooldir}/transfer.py"
    pyb = f"{tooldir}/transfer_batch.py"
    pch = os.path.relpath(T.PCH_PATH, ROOT)
    L = [
        "ninja_required_version = 1.10",
        "",
        "rule pch",
        f"  command = python3 {py} --build-pch",
        "  description = PCH $out",
        "rule xferdir",
        f"  command = python3 {pyb} --edge-dir $out $in",
        "  description = XFER $dir",
        # 1100+ dirrec paths can exceed ARG_MAX; use a response file
        "rule aggregate",
        f"  command = python3 {py} --aggregate $out @$out.rsp",
        "  rspfile = $out.rsp",
        "  rspfile_content = $recs",
        "  description = MANIFEST $out",
        "rule tripwire",
        f"  command = python3 {tooldir}/tripwire.py $out",
        "  description = TRIPWIRE",
        "",
        f"build {pch}: pch | {py}",
        "",
    ]
    bydir = {}
    for sub in subtrees:
        for src, rel in T.list_inputs(sub):
            bydir.setdefault(os.path.dirname(rel), []).append((src, rel))

    recs = []
    for d in sorted(bydir):
        label = d.replace("/", "_")
        srcs = " ".join(os.path.relpath(s, ROOT) for s, _ in bydir[d])
        outs = [os.path.relpath(os.path.join(REC_DIR, rel + ".rec.json"), ROOT)
                for _, rel in bydir[d]]
        # multi-output edge: ALL the directory's per-file recs are declared
        # outputs (written by transfer_batch), so group edges depending on
        # individual recs resolve; one Python+PCH startup serves the directory.
        L += ["build " + " $\n    ".join(outs) + f": xferdir {srcs} | {py} {pyb} {pch}",
              f"  dir = {label}"]
        recs += outs

    man = os.path.relpath(os.path.join(T.DST_ROOT, "manifest.json"), ROOT)
    L.append(f"build {man}: aggregate " + " $\n    ".join(recs))
    L.append("  recs = " + " ".join(recs))
    L += [f"build build/tripwire.ok: tripwire {man} | {tooldir}/tripwire.py",
          "default build/tripwire.ok", ""]

    out = os.path.join(ROOT, "build", "transfer.ninja")
    os.makedirs(os.path.dirname(out), exist_ok=True)
    open(out, "w").write("\n".join(L))
    print(f"{out}: {len(recs)} files in {len(bydir)} directory edges")

    # Root graph: the TRANSFER stage (transfer -> manifest -> tripwire).
    #
    # It also subninja's build/groups.ninja so that, ONCE that file has been
    # materialized by `gen_groups.py --ninja`, editing a library header reruns
    # exactly the dependent group results via the depfiles.  But it does NOT
    # build the groups stage on a fresh run: ninja only regenerates the
    # top-level file passed to -f, never a subninja'd one, so the gengroups
    # edge below never fires by itself and groups.ninja stays the placeholder.
    # After `ninja -f build/build.ninja`, run the groups stage explicitly:
    #   python3 tools/gen_groups.py --ninja && ninja -f build/groups.ninja libcis
    # (or just use `make test`, which does the whole sequence).
    groups_ninja = os.path.join(ROOT, "build", "groups.ninja")
    if not os.path.exists(groups_ninja):
        open(groups_ninja, "w").write("# generated by gen_groups.py --ninja\n")
    root = "\n".join([
        "ninja_required_version = 1.10",
        "include build/transfer.ninja",
        "",
        "rule gengroups",
        "  command = python3 tools/gen_groups.py --ninja",
        "  generator = 1",
        "  restat = 1",
        f"build build/groups.ninja: gengroups {man} | tools/gen_groups.py build/tripwire.ok",
        "subninja build/groups.ninja",
        "",
    ])
    open(os.path.join(ROOT, "build", "build.ninja"), "w").write(root)
    print("build/build.ninja: transfer graph (transfer + manifest + tripwire)")
    print("  then: python3 tools/gen_groups.py --ninja && "
          "ninja -f build/groups.ninja libcis   (or: make test)")


if __name__ == "__main__":
    main()
