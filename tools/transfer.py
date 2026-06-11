#!/usr/bin/env python3
"""transfer.py - copy AND ADAPT libc++'s test suite for the libcis target
(g++-10.2, -std=gnu++20, -fno-exceptions, -fno-rtti).

Pipeline per input file:
  1. lit gating: skip tests whose own UNSUPPORTED tags say they need
     exceptions/rtti (their purpose is the thing we cannot test).
  2. parse with libclang under the TARGET macro environment
     (-DTEST_HAS_NO_EXCEPTIONS -DTEST_HAS_NO_RTTI, but -fexceptions -frtti so
     hostile constructs still reach the AST): every try/throw/typeid/
     dynamic_cast cursor remaining in the main file is UNGUARDED, i.e. would
     not compile on the target.
  3. adapt: excise the smallest enclosing statement of each hostile construct
     (a throw-behavior check is untestable without exceptions; the rest of the
     test survives).  If a construct is not inside a removable statement, or
     too much of the test would be removed, skip the file with a recorded
     reason instead of producing a gutted test.
  4. isolate: wrap runs of wrappable top-level decls (including main) in
     `namespace libcis_ns_<slug> { ... } using namespace ...;` so many files
     link/consolidate together.  std:: reopens / explicit specializations stay
     unwrapped (decided from semantic_parent, not text).  main is kept as-is;
     the consolidator calls libcis_ns_<slug>::main(...) qualified.
  5. verify: re-parse the output; any new hard diagnostic fails the transfer.

gcc-10.2 *compiler feature* gaps are NOT an AST question: they are
discriminated by the g++-10 + system-libstdc++ backend (a test red there too
is a compiler limitation, not a libcis bug).

Usage:  tools/transfer.py [SUBTREE ...]      (default: numerics)
"""
import json
import os
import re
import shutil
import sys

import clang.cindex as ci

LIBCLANG = "/lib/x86_64-linux-gnu/libclang-20.so.20"
ci.Config.set_library_file(LIBCLANG)

LIBCXX = "/home/user/llvm-project/libcxx/test"
SRC_STD = os.path.join(LIBCXX, "std")
SRC_SUPPORT = os.path.join(LIBCXX, "support")
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DST_ROOT = os.path.join(ROOT, "test", "std")
DST_SUPPORT = os.path.join(DST_ROOT, "support")

# Parse with exceptions/rtti ON (so hostile constructs appear in the AST) but
# with the harness macros predefined as on the target (so the test's own
# #ifndef TEST_HAS_NO_EXCEPTIONS guarded arms drop out, like they will under
# g++-10 -fno-exceptions).  Whatever hostile construct survives preprocessing
# here is exactly what breaks the target build -> that is what we adapt.
PARSE_ARGS = [
    "-std=gnu++2c", "-stdlib=libc++",
    "-DTEST_HAS_NO_EXCEPTIONS", "-DTEST_HAS_NO_RTTI",
    "-I", SRC_SUPPORT,
]

# A PCH of every top-level libc++ header, built once: each of the ~10k test
# parses (plus its verify re-parse) then skips the std headers entirely
# instead of re-chewing them.
PCH_PATH = os.path.join(ROOT, "build", "transfer.pch")
PER_FILE_TIMEOUT = 60  # seconds; a stuck/crashed libclang parse must not stall the suite


def build_pch():
    cand = ["/usr/lib/llvm-20/include/c++/v1", "/usr/include/c++/v1"]
    inc_dir = next((d for d in cand if os.path.isdir(d)), None)
    if inc_dir is None:
        return []  # no libc++ headers found; parse without PCH
    hdrs = sorted(h for h in os.listdir(inc_dir)
                  if "." not in h and not h.startswith("__")
                  and os.path.isfile(os.path.join(inc_dir, h)))
    mega = os.path.join(ROOT, "build", "transfer_mega.h")
    os.makedirs(os.path.dirname(mega), exist_ok=True)
    with open(mega, "w") as fh:
        fh.write("".join(f"#include <{h}>\n" for h in hdrs))
        fh.write('#include "test_macros.h"\n')
    idx = ci.Index.create()
    # -x c++-header: libclang would otherwise treat the .h as a C header,
    # which is fatal together with -std=gnu++2c.
    tu = idx.parse(mega, args=["-x", "c++-header"] + PARSE_ARGS,
                   options=ci.TranslationUnit.PARSE_INCOMPLETE)
    hard = [d for d in tu.diagnostics if d.severity >= ci.Diagnostic.Error]
    if hard:
        print(f"PCH: build failed ({hard[0]}); parsing without PCH", flush=True)
        return []
    tu.save(PCH_PATH)
    return ["-include-pch", PCH_PATH]

# ---------------------------------------------------------------------------
# lit feature model: features TRUE under our (newest-vendor) config.
# ---------------------------------------------------------------------------
TRUE_FEATURES = {
    "c++26", "c++23", "c++20",
    "std-at-least-c++17", "std-at-least-c++20", "std-at-least-c++23",
    "std-at-least-c++26",
    "long_tests", "has-fconstexpr-steps", "has-unix-headers",
    "locale.en_US.UTF-8", "locale.fr_FR.UTF-8", "locale.ru_RU.UTF-8",
    "locale.zh_CN.UTF-8", "locale.ja_JP.UTF-8", "locale.cs_CZ.ISO8859-2",
    "has-1024-bit-extended-precision", "linux", "x86_64",
}
FALSE_STD_MODES = {"c++03", "c++11", "c++14", "c++17"}

# Features our build actually enables (see check.sh): a test that declares
# itself UNSUPPORTED under these can NOT be adapted -- its purpose is the
# feature we turned off -- so it is skipped, with the reason recorded.
ACTIVE_NO_FEATURES = {"no-exceptions", "no-rtti"}


def feature_true(feat: str) -> bool:
    feat = feat.strip()
    if feat in TRUE_FEATURES:
        return True
    if feat in ACTIVE_NO_FEATURES:
        return True
    if feat in FALSE_STD_MODES:
        return False
    return False


def eval_clause(clause: str) -> bool:
    expr = re.sub(r"\{\{.*?\}\}", "__WILD__", clause)
    tokens = re.findall(r"&&|\|\||!|\(|\)|[A-Za-z0-9_.:=+\-]+", expr)
    py = " ".join(
        {"&&": "and", "||": "or", "!": "not", "(": "(", ")": ")"}.get(
            t, ("True" if feature_true(t) else "False"))
        for t in tokens
    )
    try:
        return bool(eval(py, {"__builtins__": {}}, {}))
    except Exception:
        return False


def parse_directives(text: str):
    d = {"UNSUPPORTED": [], "REQUIRES": [], "XFAIL": [],
         "FLAGS": [], "FILE_DEPENDENCIES": []}
    for line in text.splitlines():
        m = re.match(r"\s*//\s*(UNSUPPORTED|REQUIRES|XFAIL):\s*(.*)", line)
        if m:
            for clause in m.group(2).split(","):
                clause = clause.strip()
                if clause:
                    d[m.group(1)].append(clause)
            continue
        m = re.match(r"\s*//\s*ADDITIONAL_COMPILE_FLAGS(?:\(([^)]*)\))?:\s*(.*)", line)
        if m:
            d["FLAGS"].append((m.group(1), m.group(2).strip()))
            continue
        m = re.match(r"\s*//\s*FILE_DEPENDENCIES:\s*(.*)", line)
        if m:
            d["FILE_DEPENDENCIES"].append(m.group(1).strip())
    return d


def decide(text: str):
    """(transfer:bool, reason:str, extra_flags:[str])."""
    d = parse_directives(text)
    if d["FILE_DEPENDENCIES"]:
        return False, "file-dependencies", []
    for clause in d["UNSUPPORTED"]:
        bare = clause.strip()
        if bare in ACTIVE_NO_FEATURES:
            return False, f"unsupported:{bare}", []
        if any(nf in bare for nf in ACTIVE_NO_FEATURES) and eval_clause(clause):
            return False, f"unsupported:{bare}", []
    for clause in d["REQUIRES"]:
        if not eval_clause(clause):
            return False, f"requires-unmet:{clause.strip()}", []
    flags = []
    for gate, fl in d["FLAGS"]:
        if gate is None or feature_true(gate) or gate in ("gcc", "clang"):
            flags.extend(fl.split())
    return True, "ok", flags


def is_test_file(name: str) -> bool:
    return name.endswith(".pass.cpp") or name.endswith(".compile.pass.cpp")


def slug_for(rel: str) -> str:
    s = re.sub(r"\.(compile\.)?pass\.cpp$", "", rel)
    s = re.sub(r"[^A-Za-z0-9]+", "_", s).strip("_")
    return s


# ---------------------------------------------------------------------------
# AST adaptation: constructs the target cannot compile.
# ---------------------------------------------------------------------------
class TransformError(Exception):
    pass


HOSTILE = {
    ci.CursorKind.CXX_TRY_STMT: "exceptions",
    ci.CursorKind.CXX_THROW_EXPR: "exceptions",
    ci.CursorKind.CXX_TYPEID_EXPR: "rtti",
    ci.CursorKind.CXX_DYNAMIC_CAST_EXPR: "rtti",
}

# Removing more than this many statements means the test IS about the feature;
# skip it (recorded) rather than ship a hollowed-out test.
MAX_EXCISIONS = 25


def find_hostile(tu, path):
    """[(stmt_cursor_to_remove, why)] for every unguarded hostile construct in
    the main file; raises TransformError if one is not inside a removable
    statement (-> caller skips the file).  Walks ONLY the test file's own
    top-level subtrees, never the included headers' AST."""
    out = []
    stack = []

    def enclosing_stmt(node):
        # smallest ancestor (or node itself) whose parent is a CompoundStmt
        chain = stack + [node]
        for i in range(len(chain) - 1, 0, -1):
            if chain[i - 1].kind == ci.CursorKind.COMPOUND_STMT:
                return chain[i]
        return None

    def walk(cur):
        stack.append(cur)
        for ch in cur.get_children():
            if ch.kind in HOSTILE:
                stmt = enclosing_stmt(ch)
                if stmt is None:
                    raise TransformError(
                        f"unremovable {HOSTILE[ch.kind]} construct at "
                        f"{ch.location.line}:{ch.location.column}")
                out.append((stmt, HOSTILE[ch.kind]))
                continue  # whole statement goes; no need to descend
            walk(ch)
        stack.pop()

    for top in tu.cursor.get_children():
        if top.location.file is not None \
                and os.path.abspath(str(top.location.file.name)) == path:
            walk(top)
    return out


def excision_edits(text, hostile):
    """Statement removals as (start, end, replacement) edits, deduped and with
    nested extents dropped; replacement preserves the line count."""
    spans = []
    for stmt, why in hostile:
        s, e = stmt.extent.start.offset, stmt.extent.end.offset
        j = e
        while j < len(text) and text[j] in " \t":
            j += 1
        if j < len(text) and text[j] == ";":
            e = j + 1
        spans.append((s, e, why))
    spans.sort()
    kept = []
    for s, e, why in spans:
        if kept and s >= kept[-1][0] and e <= kept[-1][1]:
            continue  # nested inside an already-removed statement
        kept.append((s, e, why))
    edits, counts = [], {}
    for s, e, why in kept:
        counts[why] = counts.get(why, 0) + 1
        nl = "\n" * text.count("\n", s, e)
        edits.append((s, e, f"/* libcis: removed ({why}: untestable on target) */{nl}"))
    return edits, counts


# ---------------------------------------------------------------------------
# AST isolation: per-file namespace so files consolidate into one TU.
# ---------------------------------------------------------------------------
_UNWRAPPABLE_KINDS = {
    ci.CursorKind.NAMESPACE,
    ci.CursorKind.USING_DIRECTIVE,
    ci.CursorKind.USING_DECLARATION,
    ci.CursorKind.NAMESPACE_ALIAS,
    ci.CursorKind.LINKAGE_SPEC,
    ci.CursorKind.MACRO_DEFINITION,
    ci.CursorKind.MACRO_INSTANTIATION,
    ci.CursorKind.INCLUSION_DIRECTIVE,
}


def is_wrappable(c) -> bool:
    # only decls that introduce a name at TU scope may move into a namespace;
    # out-of-line definitions / explicit specializations (semantic_parent is
    # some other namespace, e.g. std) and namespace reopens must stay put.
    if c.kind in _UNWRAPPABLE_KINDS:
        return False
    sp = c.semantic_parent
    if sp is not None and sp.kind != ci.CursorKind.TRANSLATION_UNIT:
        return False
    return True


def all_toplevel_decls(tu, path):
    return [c for c in tu.cursor.get_children()
            if c.location.file is not None
            and os.path.abspath(str(c.location.file.name)) == path]


def find_main(tu, path):
    for c in all_toplevel_decls(tu, path):
        if c.kind == ci.CursorKind.FUNCTION_DECL and c.spelling == "main" \
                and c.is_definition() and c.result_type.spelling == "int":
            return c
    return None


def transform(text: str, tu, path: str, slug: str):
    """Returns (output_text, entry_call or None, adapt_counts)."""
    hostile = find_hostile(tu, path)
    edits, counts = excision_edits(text, hostile)
    if sum(counts.values()) > MAX_EXCISIONS:
        raise TransformError(f"adapt-too-much:{counts}")

    main_cursor = find_main(tu, path)
    entry = None
    if main_cursor is not None:
        nparams = sum(1 for p in main_cursor.get_children()
                      if p.kind == ci.CursorKind.PARM_DECL)
        args = "0, nullptr" if nparams else ""
        entry = f"libcis_ns_{slug}::main({args})"

    # wrap each maximal contiguous run of wrappable decls (main included) in
    # the per-file namespace; `using namespace` keeps unqualified/ADL lookup
    # identical for the unwrapped regions that follow.
    decls = sorted(all_toplevel_decls(tu, path), key=lambda d: d.extent.start.offset)
    ns = f"libcis_ns_{slug}"
    runs, cur = [], []
    for d in decls:
        if is_wrappable(d):
            cur.append(d)
        else:
            if cur:
                runs.append(cur)
            cur = []
    if cur:
        runs.append(cur)
    for run in runs:
        ws = run[0].extent.start.offset
        we = max(d.extent.end.offset for d in run)
        j = we
        while j < len(text) and text[j] in " \t":
            j += 1
        if j < len(text) and text[j] == ";":
            we = j + 1
        edits.append((ws, ws, f"namespace {ns} {{ // libcis\n"))
        edits.append((we, we, f"\n}} using namespace {ns}; // libcis\n"))

    edits.sort(key=lambda e: (e[0], e[1]), reverse=True)
    out = text
    for s, e, repl in edits:
        out = out[:s] + repl + out[e:]

    note = ""
    if counts:
        note = f"// libcis: adapted for target ({counts}).\n"
    return (f"// transferred+adapted from libc++ by tools/transfer.py (slug={slug}).\n"
            + note + out, entry, counts)


def parse_args():
    extra = ["-include-pch", PCH_PATH] if os.path.exists(PCH_PATH) else []
    return PARSE_ARGS + extra


def verify(out_text: str, ref_path: str):
    """Re-parse the rewritten source; transfer fails on any hard diagnostic."""
    idx = ci.Index.create()
    tu = idx.parse(ref_path, args=parse_args(), unsaved_files=[(ref_path, out_text)])
    return [d for d in tu.diagnostics if d.severity >= ci.Diagnostic.Error]


def list_inputs(sub):
    root = os.path.join(SRC_STD, sub)
    out = []
    for dirpath, _dirs, files in os.walk(root):
        for fn in sorted(files):
            if not is_test_file(fn):
                continue
            src = os.path.abspath(os.path.join(dirpath, fn))
            out.append((src, os.path.relpath(src, SRC_STD)))
    return sorted(out, key=lambda t: t[1])


_WORKER_IDX = None


def _worker_idx():
    global _WORKER_IDX
    if _WORKER_IDX is None:
        _WORKER_IDX = ci.Index.create()
    return _WORKER_IDX


def process_one(args):
    src, rel = args
    try:
        with open(src, "r", errors="replace") as fh:
            text = fh.read()
    except Exception as ex:
        return {"status": "error", "file": rel, "stage": "read", "diag": repr(ex)}

    ok, reason, flags = decide(text)
    if not ok:
        return {"status": "skipped", "file": rel, "reason": reason}

    slug = slug_for(rel)
    try:
        idx = _worker_idx()
        tu = idx.parse(src, args=parse_args())
        hard = [d for d in tu.diagnostics if d.severity >= ci.Diagnostic.Error]
        if hard:
            return {"status": "error", "file": rel, "stage": "parse",
                    "diag": str(hard[0])}
        out, entry, counts = transform(text, tu, src, slug)
        bad = verify(out, src)
        if bad:
            return {"status": "error", "file": rel, "stage": "verify",
                    "diag": str(bad[0])}
    except TransformError as ex:
        return {"status": "skipped", "file": rel, "reason": f"adapt:{ex}"}
    except Exception as ex:  # pragma: no cover
        return {"status": "error", "file": rel, "stage": "exc", "diag": repr(ex)}

    kind = "run" if (entry and not rel.endswith(".compile.pass.cpp")) else "compile"
    return {"status": "ok", "file": rel, "slug": slug, "kind": kind,
            "entry": entry, "flags": flags, "adapted": counts, "_text": out}


def copy_sibling_headers(sub):
    root = os.path.join(SRC_STD, sub)
    for dirpath, _dirs, files in os.walk(root):
        hs = [f for f in files if f.endswith((".h", ".hpp"))]
        if not hs:
            continue
        dd = os.path.join(DST_ROOT, os.path.relpath(dirpath, SRC_STD))
        os.makedirs(dd, exist_ok=True)
        for f in hs:
            shutil.copyfile(os.path.join(dirpath, f), os.path.join(dd, f))


def load_manifest():
    p = os.path.join(DST_ROOT, "manifest.json")
    if os.path.exists(p):
        with open(p) as fh:
            m = json.load(fh)
        for k in ("transferred", "skipped", "errors", "subtrees"):
            m.setdefault(k, [])
        return m
    return {"transferred": [], "skipped": [], "errors": [], "subtrees": []}


def save_manifest(m):
    tmp = os.path.join(DST_ROOT, "manifest.json.tmp")
    with open(tmp, "w") as fh:
        json.dump(m, fh, indent=1)
    os.replace(tmp, os.path.join(DST_ROOT, "manifest.json"))


def run_pool(inputs, jobs, sub):
    """Process inputs in a worker pool that cannot stall the suite: spawn
    start-method (libclang threads make fork() unsafe), ORDERED results with a
    per-file timeout -- a hung parse is attributed to its input, recorded as
    an error, and the pool is rebuilt to continue with the rest."""
    import multiprocessing as mp
    ctx = mp.get_context("spawn")
    results = []
    i = 0
    while i < len(inputs):
        pool = ctx.Pool(processes=jobs)
        it = pool.imap(process_one, inputs[i:], chunksize=1)
        try:
            while i < len(inputs):
                try:
                    rec = it.next(timeout=PER_FILE_TIMEOUT)
                except mp.TimeoutError:
                    rec = {"status": "error", "file": inputs[i][1],
                           "stage": "timeout",
                           "diag": f"parse exceeded {PER_FILE_TIMEOUT}s"}
                    print(f"  [{sub}] TIMEOUT {inputs[i][1]}", flush=True)
                    results.append(rec)
                    i += 1
                    break  # rebuild the pool, resume after the hung input
                results.append(rec)
                i += 1
                if i % 200 == 0:
                    print(f"  [{sub}] {i}/{len(inputs)}", flush=True)
        finally:
            pool.terminate()
            pool.join()
    return results


def main():
    import argparse
    from collections import Counter

    ap = argparse.ArgumentParser()
    ap.add_argument("subtrees", nargs="*", default=["numerics"])
    ap.add_argument("-j", "--jobs", type=int, default=max(1, (os.cpu_count() or 1)))
    ap.add_argument("--fresh", action="store_true")
    args = ap.parse_args()
    subtrees = args.subtrees or ["numerics"]

    if args.fresh and os.path.isdir(DST_ROOT):
        shutil.rmtree(DST_ROOT)
    os.makedirs(DST_ROOT, exist_ok=True)
    if not os.path.isdir(DST_SUPPORT):
        shutil.copytree(SRC_SUPPORT, DST_SUPPORT)

    import time
    t0 = time.time()
    if build_pch():
        print(f"PCH built in {time.time()-t0:.1f}s -> {PCH_PATH}", flush=True)

    manifest = load_manifest()

    def in_target(rel):
        return any(rel == sub or rel.startswith(sub + "/") for sub in subtrees)
    for k in ("transferred", "skipped", "errors"):
        manifest[k] = [r for r in manifest[k] if not in_target(r["file"])]
    for sub in subtrees:
        if sub not in manifest["subtrees"]:
            manifest["subtrees"].append(sub)

    n_adapted = 0
    for sub in subtrees:
        inputs = list_inputs(sub)
        print(f"[{sub}] {len(inputs)} inputs, -j{args.jobs}", flush=True)
        nt = ns = ne = 0
        results = run_pool(inputs, args.jobs, sub)
        for rec in results:
            st = rec["status"]
            if st == "ok":
                out = rec.pop("_text")
                dst = os.path.join(DST_ROOT, rec["file"])
                os.makedirs(os.path.dirname(dst), exist_ok=True)
                with open(dst, "w") as fh:
                    fh.write(out)
                manifest["transferred"].append(
                    {k: rec[k] for k in
                     ("file", "slug", "kind", "entry", "flags", "adapted")})
                nt += 1
                if rec["adapted"]:
                    n_adapted += 1
            elif st == "skipped":
                manifest["skipped"].append({"file": rec["file"], "reason": rec["reason"]})
                ns += 1
            else:
                manifest["errors"].append(
                    {"file": rec["file"], "stage": rec["stage"], "diag": rec["diag"]})
                ne += 1
        copy_sibling_headers(sub)
        save_manifest(manifest)
        print(f"[{sub}] transferred={nt} skipped={ns} errors={ne} "
              f"(of {len(inputs)})", flush=True)

    nt, ns, ne = (len(manifest["transferred"]), len(manifest["skipped"]),
                  len(manifest["errors"]))
    na = sum(1 for r in manifest["transferred"] if r.get("adapted"))
    print(f"TOTAL transferred={nt} (adapted={na}) skipped={ns} errors={ne}")
    for r, c in Counter(s["reason"].split(":")[0] for s in manifest["skipped"]).most_common(12):
        print(f"   skip[{r}] = {c}")
    for r, c in Counter(e["stage"] for e in manifest["errors"]).most_common(12):
        print(f"   err[{r}] = {c}")


if __name__ == "__main__":
    main()
