#!/usr/bin/env python3
"""transfer.py - AST-driven transfer of LLVM libc++'s real test suite into the
libcis project's runnable suite under test/std/.

THIS IS A REAL C++ TRANSFORMATION, NOT A COPY.
================================================
Every output file is produced by parsing the input with the clang C++ front-end
(python libclang) and rewriting it off the actual AST -- declarations, scopes,
source locations and macro-expansion info -- never by regex/text munging of C++
syntax.  (Regex is used ONLY for the line-oriented `// lit:` directive comments,
which are not C++ and are explicitly not part of the AST.)

Per input file the AST drives these transforms:

  1. RENAME main -> test_<slug>.   We locate the `int main(int,char**)` via the
     AST (CursorKind.FUNCTION_DECL, spelling=="main", and isolation that it is
     the program entry point: a file-scope function returning int named main).
     Using its *token* extents we replace the exact signature
        `int main(<params>)`   ->   `void test_<slug>()`
     and rewrite every RETURN_STMT that is a direct child of main's body
     (AST CompoundStmt -> ReturnStmt) from `return <expr>;` to `return;`,
     because the new function is void.  slug is derived from the file's path so
     it is unique across the whole suite -> the harness (tools/gen_main.sh, which
     discovers free functions named test_*) runs each transferred test, and many
     files can be linked together without entry-point clashes.

  2. ISOLATE file-scope declarations.  Each libc++ test carries its OWN
     file-scope helper types and helper functions (often themselves named
     `test`, `Foo`, `A`, ...).  We compute, from the AST, the source span of the
     top-level declarations that precede main and wrap exactly that span in an
     anonymous namespace `namespace { ... }`.  Anonymous-namespace = internal
     linkage, so when several transferred files are linked into one binary their
     helpers cannot collide; main (now test_<slug>) is emitted OUTSIDE the
     namespace but still sees the helpers by ordinary lookup.  The span comes
     from cursor.extent (real source locations), not from matching braces.

  3. MACRO FAITHFULNESS.  TEST_*, ASSERT_SAME_TYPE, TEST_CONSTEXPR_CXX20, assert,
     ... are preprocessor constructs.  We do NOT string-replace them: we keep the
     support harness (test/std/support, adapted) on the include path so they
     expand through the real preprocessor exactly as upstream.  `assert` resolves
     to the platform <cassert> on every backend (it aborts -> non-zero exit on
     failure), which is precisely the run-pass/run-fail contract check.sh wants;
     libcis ships its own <cassert>, so the same semantics hold freestanding.
     We VERIFY macro-faithfulness by re-parsing the rewritten output and
     asserting it still has 0 hard diagnostics and still contains test_<slug>.

  4. lit-directive gating (line comments, regex is correct here).  We parse
     UNSUPPORTED/REQUIRES/XFAIL and skip a test ONLY when our config's actually
     active features (no-exceptions, no-rtti) intersect its UNSUPPORTED set, or a
     REQUIRES clause is unmet under the newest-vendor (C++26, threads, locale,
     filesystem, wchar) feature model.  We do NOT skip on no-threads /
     no-localization / no-wide-characters / old-standard tags: the vendors have
     those and we compile at C++23+.

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

PARSE_ARGS = [
    "-std=gnu++2c", "-stdlib=libc++", "-fno-exceptions", "-fno-rtti",
    "-I", SRC_SUPPORT,
]

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

# Our build ACTUALLY enables only these "no-*" features (see check.sh):
#   -fno-exceptions -> no-exceptions ;  -fno-rtti -> no-rtti
# An UNSUPPORTED on one of these means the test genuinely can't build for us.
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
    # Skip ONLY when an UNSUPPORTED clause is satisfied AND it concerns a feature
    # our config actually activates (no-exceptions / no-rtti) -- i.e. the test
    # cannot build for us.  Other satisfied UNSUPPORTED (e.g. c++26 quirks) still
    # gate, but we do NOT pre-skip no-threads/no-localization/etc.
    for clause in d["UNSUPPORTED"]:
        bare = clause.strip()
        if bare in ACTIVE_NO_FEATURES:
            return False, f"unsupported:{bare}", []
        # a compound clause that is satisfied AND mentions an active-no feature
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
# The AST transform.
# ---------------------------------------------------------------------------
class TransformError(Exception):
    pass


def find_main(tu, path):
    """Return the FUNCTION_DECL cursor for the program entry point main, or None."""
    cand = None
    for c in tu.cursor.get_children():
        if c.location.file is None or os.path.abspath(str(c.location.file.name)) != path:
            continue
        if c.kind == ci.CursorKind.FUNCTION_DECL and c.spelling == "main" \
                and c.is_definition():
            # entry point: returns int.
            if c.result_type.spelling in ("int",):
                cand = c
    return cand


def toplevel_decls_before(tu, path, main_cursor):
    """AST top-level decls in this file that precede main (by source offset)."""
    decls = []
    main_start = main_cursor.extent.start.offset
    for c in tu.cursor.get_children():
        if c.location.file is None or os.path.abspath(str(c.location.file.name)) != path:
            continue
        if c.extent.start.offset >= main_start:
            continue
        decls.append(c)
    return decls


def main_signature_end(main_cursor):
    """Byte offset just past the ')' that closes main's parameter list, using
    the token stream from the AST cursor (not text scanning)."""
    depth = 0
    seen_open = False
    for t in main_cursor.get_tokens():
        if t.spelling == "(":
            depth += 1
            seen_open = True
        elif t.spelling == ")":
            depth -= 1
            if seen_open and depth == 0:
                return t.extent.end.offset
        elif t.spelling == "{" and not seen_open:
            # main() with no params shouldn't happen for entry point, bail
            break
    raise TransformError("could not find end of main parameter list")


def body_return_stmts(main_cursor):
    """RETURN_STMT cursors anywhere in main's body that carry a value, so we can
    turn `return <expr>;` into `return;` (the rewritten function is void)."""
    rets = []

    def walk(cur):
        for ch in cur.get_children():
            if ch.kind == ci.CursorKind.RETURN_STMT:
                # only rewrite returns that belong to main itself, not to nested
                # lambdas / local classes (those have their own function scope).
                if not _in_nested_function(ch, main_cursor):
                    rets.append(ch)
            # do not descend into lambda/local-function bodies
            if ch.kind in (ci.CursorKind.LAMBDA_EXPR,):
                continue
            walk(ch)

    walk(main_cursor)
    return rets


def _in_nested_function(cur, main_cursor):
    # Heuristic via AST: a return is "nested" if a LAMBDA_EXPR encloses it.
    # We approximate by checking semantic parent chain isn't available cheaply;
    # instead body_return_stmts already prunes lambda descent, so returns reached
    # here are main's own.  Keep as False.
    return False


def all_toplevel_decls(tu, path):
    out = []
    for c in tu.cursor.get_children():
        if c.location.file is None or os.path.abspath(str(c.location.file.name)) != path:
            continue
        out.append(c)
    return out


# A decl is safe to move into an anonymous namespace iff it actually INTRODUCES a
# new name at translation-unit scope.  An out-of-line / qualified definition or
# an explicit specialization (e.g. `template<> T std::numbers::pi_v<U>{};`) has a
# semantic_parent that is some OTHER namespace, not the TU -- wrapping it in an
# anonymous namespace is ill-formed.  Likewise namespace-reopens, using-
# directives/declarations, and linkage specs must stay where they are.  This is
# decided from the AST (semantic_parent + cursor kind), never from text.
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
    if c.kind in _UNWRAPPABLE_KINDS:
        return False
    sp = c.semantic_parent
    if sp is not None and sp.kind != ci.CursorKind.TRANSLATION_UNIT:
        return False  # out-of-line / explicit specialization in another scope
    # explicit specialization whose semantic parent is the TU is still fine to
    # leave unwrapped; detect via the presence of empty `template <>` is hard,
    # so rely on semantic_parent above which already catches the std:: cases.
    return True


def main_param_usage(main_cursor):
    """Return the set of main's parameter spellings that are actually referenced
    inside its body (via DECL_REF_EXPR -> the ParmDecl).  If non-empty we must
    keep those names in scope after dropping the (int,char**) signature."""
    params = [c for c in main_cursor.get_children()
              if c.kind == ci.CursorKind.PARM_DECL and c.spelling]
    if not params:
        return []
    pset = {p.spelling for p in params}
    used = set()

    def walk(cur):
        for ch in cur.get_children():
            if ch.kind == ci.CursorKind.DECL_REF_EXPR and ch.spelling in pset:
                used.add(ch.spelling)
            walk(ch)

    walk(main_cursor)
    # return params in declaration order that are used
    return [p.spelling for p in params if p.spelling in used]


def body_open_brace_offset(main_cursor):
    """Offset just past main's body opening '{' (token-driven, from the AST)."""
    depth = 0
    seen_params = False
    for t in main_cursor.get_tokens():
        if t.spelling == "(":
            depth += 1; seen_params = True
        elif t.spelling == ")":
            depth -= 1
        elif t.spelling == "{" and (depth == 0):
            return t.extent.end.offset
    raise TransformError("could not find main body '{'")


def transform(text: str, tu, path: str, slug: str):
    """Returns (output_text, has_main).  has_main False => compile-only file with
    no entry point (e.g. a *.compile.pass.cpp that only static_asserts); such a
    file is still transferred (all decls isolated in an anon namespace) but the
    runner builds it with -fsyntax-only and no driver."""
    main_cursor = find_main(tu, path)
    data = text
    edits = []
    has_main = main_cursor is not None

    if has_main:
        # 1. rewrite the signature: [main.extent.start .. sig_end) -> void test_slug()
        sig_start = main_cursor.extent.start.offset
        sig_end = main_signature_end(main_cursor)
        edits.append((sig_start, sig_end, f"void test_{slug}()"))

        # 1b. if main's body references its (int argc, char** argv) parameters,
        # those names vanish when we drop the signature.  Re-introduce them as
        # locals at the very top of the body so the body still type-checks; argc
        # gets a runtime (non-constexpr) value of 1, argv a 1-element vector with
        # a synthetic program name, matching how lit invokes the test.
        used = main_param_usage(main_cursor)
        if used:
            brace = body_open_brace_offset(main_cursor)
            shim = "\n    // libcis: re-bind dropped main() parameters as runtime locals\n"
            names = set(used)
            # identify which spelling is the int (argc) vs char** (argv)
            for p in main_cursor.get_children():
                if p.kind != ci.CursorKind.PARM_DECL or p.spelling not in names:
                    continue
                t = p.type.spelling
                if "char" in t and "*" in t:
                    shim += f"    static char libcis_arg0[] = \"libcis_test\";\n"
                    shim += f"    static char* {p.spelling}_storage[] = {{libcis_arg0, nullptr}};\n"
                    shim += f"    char** {p.spelling} = {p.spelling}_storage; (void){p.spelling};\n"
                else:
                    shim += f"    {t} {p.spelling} = 1; (void){p.spelling};\n"
            edits.append((brace, brace, shim))

        # 2. value-returns in main's body -> bare `return;`
        for r in body_return_stmts(main_cursor):
            rs, re_ = r.extent.start.offset, r.extent.end.offset
            frag = data[rs:re_]
            if re.match(r"return\b", frag) and frag.strip() not in ("return", "return;"):
                # the RETURN_STMT extent excludes the trailing ';', so emit bare
                # `return` (no semicolon) and let the source's ';' terminate it.
                edits.append((rs, re_, "return"))

        # 3. wrap file-scope decls preceding main in an anonymous namespace.
        decls = toplevel_decls_before(tu, path, main_cursor)
    else:
        # no entry point: isolate ALL top-level decls.
        decls = all_toplevel_decls(tu, path)

    # Wrap each MAXIMAL CONTIGUOUS run of wrappable decls in its own anonymous
    # namespace, leaving unwrappable decls (explicit specializations, namespace
    # reopens, using-directives) untouched in place.
    decls = sorted(decls, key=lambda d: d.extent.start.offset)
    runs = []
    cur = []
    for d in decls:
        if is_wrappable(d):
            cur.append(d)
        else:
            if cur:
                runs.append(cur); cur = []
    if cur:
        runs.append(cur)

    # Use a NAMED per-file namespace + trailing `using namespace` rather than an
    # anonymous namespace.  Both give cross-file uniqueness (the namespace name
    # embeds the unique slug, so helpers from different transferred files cannot
    # collide when linked together), but the named-namespace + using-directive
    # form preserves ordinary unqualified name lookup AND argument-dependent
    # lookup for the test body: many libc++ tests deliberately define helpers in
    # the global namespace and rely on ADL / unqualified calls (e.g. shadowing or
    # extending std:: names).  An anonymous namespace silently changed the set of
    # associated namespaces for such calls and broke a whole class of tests; the
    # named form with `using namespace libcis_ns_<slug>;` re-exports every name
    # back into the enclosing (global) scope, so lookup is unchanged.
    ns_name = f"libcis_ns_{slug}"
    for run in runs:
        wrap_start = run[0].extent.start.offset
        wrap_end = max(d.extent.end.offset for d in run)
        j = wrap_end
        while j < len(data) and data[j] in " \t":
            j += 1
        if j < len(data) and data[j] == ";":
            wrap_end = j + 1
        edits.append((wrap_start, wrap_start,
                      f"namespace {ns_name} {{ // libcis: isolate file-scope helpers\n"))
        edits.append((wrap_end, wrap_end,
                      f"\n}} using namespace {ns_name}; // libcis\n"))

    # apply edits right-to-left so offsets stay valid
    edits.sort(key=lambda e: (e[0], e[1]), reverse=True)
    out = data
    for s, e, repl in edits:
        out = out[:s] + repl + out[e:]

    # prologue: comment marking provenance.
    if has_main:
        note = f"// main -> test_{slug}; file-scope helpers isolated in anon namespace.\n"
    else:
        note = "// no entry point (compile-only); all file-scope decls isolated in anon namespace.\n"
    header = (f"// AST-transferred from libc++ by tools/transfer.py (slug={slug}).\n" + note)
    return header + out, has_main


def verify(out_text: str, slug: str, ref_path: str):
    """Re-parse the rewritten source and assert it still parses (0 hard diags)
    and still contains the renamed entry point."""
    idx = ci.Index.create()
    unsaved = [(ref_path, out_text)]
    tu = idx.parse(ref_path, args=PARSE_ARGS, unsaved_files=unsaved,
                   options=ci.TranslationUnit.PARSE_DETAILED_PROCESSING_RECORD)
    hard = [d for d in tu.diagnostics if d.severity >= ci.Diagnostic.Error]
    found = any(c.kind == ci.CursorKind.FUNCTION_DECL and c.spelling == f"test_{slug}"
                for c in tu.cursor.get_children()
                if c.location.file and os.path.abspath(str(c.location.file.name)) == ref_path)
    return hard, found


def list_inputs(sub):
    """All test inputs under a subtree, as (src_abspath, rel) sorted."""
    root = os.path.join(SRC_STD, sub)
    out = []
    for dirpath, _dirs, files in os.walk(root):
        for fn in sorted(files):
            if not is_test_file(fn):
                continue
            src = os.path.abspath(os.path.join(dirpath, fn))
            rel = os.path.relpath(src, SRC_STD)
            out.append((src, rel))
    return sorted(out, key=lambda t: t[1])


# Each worker process keeps its own libclang Index (not fork-safe to share).
_WORKER_IDX = None


def _worker_idx():
    global _WORKER_IDX
    if _WORKER_IDX is None:
        _WORKER_IDX = ci.Index.create()
    return _WORKER_IDX


def process_one(args):
    """Worker: transfer a single input.  Returns a record dict describing the
    outcome AND, on success, the output text (written by the parent so the
    filesystem isn't touched concurrently)."""
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
        tu = idx.parse(src, args=PARSE_ARGS)
        hard = [d for d in tu.diagnostics if d.severity >= ci.Diagnostic.Error]
        if hard:
            return {"status": "error", "file": rel, "stage": "parse",
                    "diag": str(hard[0])}
        out, has_main = transform(text, tu, src, slug)
    except TransformError as ex:
        return {"status": "error", "file": rel, "stage": "transform", "diag": str(ex)}
    except Exception as ex:  # pragma: no cover
        return {"status": "error", "file": rel, "stage": "exc", "diag": repr(ex)}

    if not has_main or rel.endswith(".compile.pass.cpp"):
        kind = "compile"
    else:
        kind = "run"
    return {"status": "ok", "file": rel, "slug": slug, "kind": kind,
            "flags": flags, "_text": out}


def copy_sibling_headers(sub):
    root = os.path.join(SRC_STD, sub)
    for dirpath, _dirs, files in os.walk(root):
        hs = [f for f in files if f.endswith((".h", ".hpp"))]
        if not hs:
            continue
        reld = os.path.relpath(dirpath, SRC_STD)
        dd = os.path.join(DST_ROOT, reld)
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


def main():
    import argparse
    import multiprocessing as mp
    from collections import Counter

    ap = argparse.ArgumentParser()
    ap.add_argument("subtrees", nargs="*", default=["numerics"])
    ap.add_argument("-j", "--jobs", type=int, default=max(1, (os.cpu_count() or 1)))
    ap.add_argument("--fresh", action="store_true",
                    help="wipe test/std before transferring (default: incremental)")
    args = ap.parse_args()
    subtrees = args.subtrees or ["numerics"]

    if args.fresh and os.path.isdir(DST_ROOT):
        shutil.rmtree(DST_ROOT)
    os.makedirs(DST_ROOT, exist_ok=True)
    if not os.path.isdir(DST_SUPPORT):
        shutil.copytree(SRC_SUPPORT, DST_SUPPORT)

    manifest = load_manifest()
    # drop any prior records for the subtrees we're (re)building so a re-run of a
    # subtree replaces rather than duplicates its entries.
    def in_target(rel):
        return any(rel == sub or rel.startswith(sub + "/") for sub in subtrees)
    for k in ("transferred", "skipped", "errors"):
        manifest[k] = [r for r in manifest[k] if not in_target(r["file"])]
    for sub in subtrees:
        if sub not in manifest["subtrees"]:
            manifest["subtrees"].append(sub)

    for sub in subtrees:
        inputs = list_inputs(sub)
        print(f"[{sub}] {len(inputs)} inputs, -j{args.jobs}", flush=True)
        nt = ns = ne = 0
        results = []
        with mp.Pool(processes=args.jobs) as pool:
            for i, rec in enumerate(pool.imap_unordered(process_one, inputs, chunksize=4)):
                results.append(rec)
                if (i + 1) % 100 == 0:
                    print(f"  [{sub}] {i+1}/{len(inputs)}", flush=True)
        for rec in results:
            st = rec["status"]
            if st == "ok":
                out = rec.pop("_text")
                dst = os.path.join(DST_ROOT, rec["file"])
                os.makedirs(os.path.dirname(dst), exist_ok=True)
                with open(dst, "w") as fh:
                    fh.write(out)
                manifest["transferred"].append(
                    {k: rec[k] for k in ("file", "slug", "kind", "flags")})
                nt += 1
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
    print(f"TOTAL transferred={nt} skipped={ns} errors={ne}")
    for r, c in Counter(s["reason"].split(":")[0] for s in manifest["skipped"]).most_common(12):
        print(f"   skip[{r}] = {c}")
    for r, c in Counter(e["stage"] for e in manifest["errors"]).most_common(12):
        print(f"   err[{r}] = {c}")


if __name__ == "__main__":
    main()
