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

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import config as cfg  # noqa: E402

# Prefer the bootstrapped libclang wheel (toolchain/pylibs holds the cindex.py
# bindings and the self-contained libclang.so); a system `pip install libclang`
# still works as a fallback.
if os.path.isdir(cfg.PYLIBS_DIR):
    sys.path.insert(0, cfg.PYLIBS_DIR)

try:
    import clang.cindex as ci
except ModuleNotFoundError:
    raise SystemExit(
        "python libclang bindings not found (`import clang.cindex` failed).\n"
        "  Run `make bootstrap` (installs them into toolchain/pylibs), or\n"
        "  `pip install libclang` system-wide.\n"
        "  (only tools/transfer.py needs them; the test gate does not.)")

# libclang is global state in cindex; bind it lazily (only when we actually
# parse) so importing this module -- e.g. from tools/gen_transfer.py just to
# generate the build graph -- does not require libclang to be installed.
_LIBCLANG_BOUND = False


def _ensure_libclang():
    global _LIBCLANG_BOUND
    if not _LIBCLANG_BOUND:
        ci.Config.set_library_file(cfg.find_libclang())
        _LIBCLANG_BOUND = True


ROOT = cfg.ROOT
# The libc++ test corpus this transfer rewrites from (override with $LIBCXX).
LIBCXX = cfg.libcxx_test_dir()
SRC_STD = os.path.join(LIBCXX, "std")
SRC_SUPPORT = os.path.join(LIBCXX, "support")
DST_ROOT = os.path.join(ROOT, "test", "std")
DST_SUPPORT = os.path.join(DST_ROOT, "support")

# Parse with exceptions/rtti ON (so hostile constructs appear in the AST) but
# with the harness macros predefined as on the target (so the test's own
# #ifndef TEST_HAS_NO_EXCEPTIONS guarded arms drop out, like they will under
# g++-10 -fno-exceptions).  Whatever hostile construct survives preprocessing
# here is exactly what breaks the target build -> that is what we adapt.
# gnu++20: the dialect the tests will actually see on the target.  TEST_STD_VER
# becomes 20, so >=23/26 arms drop out of the AST exactly as they will under
# g++-10.  Tests of post-C++20 library features are SKIPPED (reason recorded in
# the manifest as requires-unmet/unsupported); when libcis implements such a
# feature on the C++20 dialect, re-transfer its subtree with --std to lift the
# gate deliberately rather than wholesale.
STD_MODE = "c++20"
# Parse against the INSTALLED clang-20 libc++ (the in-tree headers need
# clang>=21).  The installed reference is older than the tests, so tests of
# post-release fixes can carry SEMANTIC diagnostics (failing constexpr
# static_asserts); those do NOT block transfer -- the AST is structurally
# intact and adaptation works -- they are recorded as soft_errors and the
# backends judge the test.  Only structural failures (missing headers) block.
PARSE_ARGS = [
    "-std=gnu++20", "-stdlib=libc++",
    "-DTEST_HAS_NO_EXCEPTIONS", "-DTEST_HAS_NO_RTTI",
    "-I", SRC_SUPPORT,
]
# libclang often miscomputes its own resource dir, so the clang builtin headers
# (stddef.h, ...) are missing from the parse.  Hand clang the resource-dir ROOT
# with -resource-dir and let the frontend order it correctly (libc++ first, then
# builtins, then the C lib) -- forcing it on with -isystem instead shadows
# libc++'s own <stddef.h> and trips its <cstddef> guard.  Override the dir with
# $CLANG_RESOURCE_DIR.
_RESOURCE_DIR = cfg.clang_resource_dir()
if _RESOURCE_DIR:
    PARSE_ARGS += ["-resource-dir", _RESOURCE_DIR]

# Point clang at the libc++ headers explicitly.  -stdlib=libc++ alone only finds
# them when libc++ is installed at a path the clang/libclang in use knows about;
# the bootstrapped toolchain ships them as a bare directory (the llvm-project
# checkout), so name it with -isystem.  This goes AFTER -resource-dir: the libc++
# search dir then precedes the builtins, so libc++'s <cstddef> #include_next finds
# the clang <stddef.h> rather than shadowing it.
_LIBCXX_INC = cfg.libcxx_include_dir()
if _LIBCXX_INC:
    PARSE_ARGS += ["-isystem", _LIBCXX_INC]

# A PCH of every top-level libc++ header, built once: each of the ~10k test
# parses (plus its verify re-parse) then skips the std headers entirely
# instead of re-chewing them.
PCH_PATH = cfg.PCH_PATH
PER_FILE_TIMEOUT = 60  # seconds; a stuck/crashed libclang parse must not stall the suite


def build_pch():
    _ensure_libclang()
    inc_dir = cfg.libcxx_include_dir()
    if inc_dir is None:
        tried = cfg._libcxx_include_candidates()
        sys.exit(
            "PCH: no libc++ headers found. The transfer parse REQUIRES a libc++\n"
            "include directory (parsing without PCH is not supported).\n"
            "  Tried (in order):\n"
            + "".join(f"    {c}\n" for c in tried) +
            "  Install libc++ headers (e.g. `apt install libc++-20-dev`) or set\n"
            "  LIBCXX_INCLUDE to point at a libc++ `include/c++/v1` directory.")
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
        sys.exit(
            f"PCH: build failed parsing libc++ headers at {inc_dir}\n"
            f"  first error: {hard[0]}\n"
            "  The transfer parse REQUIRES a working PCH. Check that\n"
            "  LIBCXX_INCLUDE points at a libc++ header tree the configured\n"
            f"  clang ({cfg.CXX_LIBCXX}) can parse.")
    tu.save(PCH_PATH)
    return ["-include-pch", PCH_PATH]

# ---------------------------------------------------------------------------
# lit feature model: features TRUE under our (newest-vendor) config.
# ---------------------------------------------------------------------------
TRUE_FEATURES = {
    "c++20",
    "std-at-least-c++17", "std-at-least-c++20",
    "long_tests", "has-fconstexpr-steps", "has-unix-headers",
    "locale.en_US.UTF-8", "locale.fr_FR.UTF-8", "locale.ru_RU.UTF-8",
    "locale.zh_CN.UTF-8", "locale.ja_JP.UTF-8", "locale.cs_CZ.ISO8859-2",
    "has-1024-bit-extended-precision", "linux", "x86_64",
    # our backend IS gcc: tests marked `UNSUPPORTED: gcc` (e.g. the `import
    # std;` module tests) genuinely cannot run here and must be lit-skipped.
    "gcc",
    # libcis has no libc++-hardening: _LIBCPP_ASSERT death-tests
    # (UNSUPPORTED: libcpp-hardening-mode=none) must be lit-skipped.
    "libcpp-hardening-mode=none",
}
FALSE_STD_MODES = {"c++03", "c++11", "c++14", "c++17", "c++23", "c++26",
                   "std-at-least-c++23", "std-at-least-c++26"}

# Features our build actually enables (-fno-exceptions / -fno-rtti): a test that
# declares itself UNSUPPORTED under these can NOT be adapted -- its purpose is
# the feature we turned off -- so it is skipped, with the reason recorded.
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
    # lit semantics: any satisfied UNSUPPORTED clause gates the test.  Features
    # we don't have (no-threads, no-localization, ...) evaluate false, so those
    # tags do not skip; std modes other than c++20 evaluate false likewise.
    for clause in d["UNSUPPORTED"]:
        if eval_clause(clause):
            return False, f"unsupported:{clause.strip()}", []
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
    ci.CursorKind.LINKAGE_SPEC,          # extern "C" -> genuinely global symbols
    ci.CursorKind.MACRO_DEFINITION,
    ci.CursorKind.MACRO_INSTANTIATION,
    ci.CursorKind.INCLUSION_DIRECTIVE,
}


def is_wrappable(c) -> bool:
    # Wrap the whole file body in the per-file namespace so consolidated
    # siblings can't collide.  libc++ tests love a named OR anonymous helper
    # namespace (`namespace N11 {...}`, `namespace {...}`) plus `using
    # namespace`/aliases referring to it -- all of that must move TOGETHER into
    # libcis_ns_<slug>, or two files reopening the same name (or both using an
    # anonymous namespace) redefine each other when concatenated.
    if c.kind == ci.CursorKind.NAMESPACE:
        # Only a `std` reopen/specialization must stay at TU scope (you cannot
        # specialize std:: from inside another namespace); named + anonymous
        # user namespaces are wrapped.
        return c.spelling != "std"
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


def _pp_directive(line):
    s = line.lstrip()
    if not s.startswith("#"):
        return None
    d = s[1:].lstrip()
    for kw in ("ifdef", "ifndef", "if", "elif", "else", "endif"):
        if d == kw or d.startswith(kw + " ") or d.startswith(kw + "("):
            return kw
    return None


def _pp_unconditional_span(text, ws, we):
    """Widen [ws, we) so both endpoints sit at preprocessor-conditional depth 0.

    ws moves back before the outermost #if open at ws; we moves forward past the
    matching #endif of any conditional still open at we. Pure text scan -- never
    changes which decls are wrapped, only avoids splitting a #if/#endif pair.
    """
    lines = []
    off = 0
    for line in text.split("\n"):
        lines.append((off, line))
        off += len(line) + 1
    # Stack of #if line-start offsets that are still open at ws.
    stack = []
    for lo, line in lines:
        if lo >= ws:
            break
        d = _pp_directive(line)
        if d in ("if", "ifdef", "ifndef"):
            stack.append(lo)
        elif d == "endif" and stack:
            stack.pop()
    if stack:
        ws = stack[0]
    # From the (possibly new) ws, ensure we is at depth 0: consume any #endif that
    # closes a conditional opened in [ws, we).
    depth = 0
    new_we = we
    for lo, line in lines:
        if lo < ws:
            continue
        d = _pp_directive(line)
        if lo < we:
            if d in ("if", "ifdef", "ifndef"):
                depth += 1
            elif d == "endif" and depth > 0:
                depth -= 1
        else:
            if depth == 0:
                break
            if d == "endif":
                depth -= 1
                new_we = lo + len(line) + 1
    return ws, new_we


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
        # ::main implicitly returns 0 on fall-through, but a namespaced main does
        # not -- a value-less fall-off then becomes UB (garbage exit code, e.g.
        # re.submatch swap).  Insert an explicit `return 0;` just before main's
        # closing brace; it is dead code when every path already returns, so it
        # is always safe and never changes a test's meaning.
        body = next((c for c in main_cursor.get_children()
                     if c.kind == ci.CursorKind.COMPOUND_STMT), None)
        if body is not None:
            close = body.extent.end.offset - 1  # offset of the closing '}'
            edits.append((close, close, "\n    return 0;\n"))

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
        # The AST is parsed with _LIBCPP_VERSION defined, so the first/last
        # wrappable decl can sit inside a `#ifdef _LIBCPP_VERSION` block that
        # libcis (which does not define it) drops -- leaving an unbalanced
        # `namespace {` ... `}`.  Hoist the open before, and push the close after,
        # any preprocessor conditional they fall inside, so both braces are
        # unconditional (mechanical; messages_base was the symptom).
        ws, we = _pp_unconditional_span(text, ws, we)
        # NO `using namespace` re-export: each file (its helpers AND its main)
        # stays inside libcis_ns_<slug>; the group driver calls main qualified.
        # Re-exporting dragged every file's helpers to global scope, so sibling
        # files in a consolidated TU collided (6 different `enum ClassicEnum`s
        # -> ambiguous). Within a file, unqualified lookup is unaffected (all
        # its decls share the namespace); ADL still finds operators via the
        # type's own (now namespaced) home.
        edits.append((ws, ws, f"namespace {ns} {{ // libcis\n"))
        edits.append((we, we, f"\n}} // {ns} (libcis)\n"))

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


# Match a file-scope `static_assert(IDENT());` or
# `static_assert(IDENT(), "literal");` on its own line. Used only when
# LIBCIS_SKIP_CONSTEXPR=1: the round-2 vector deliberately drops constexpr,
# so the constexpr arm of libc++'s standard test pattern
#     int main(){ tests(); static_assert(tests()); return 0; }
# stops compiling and the whole TU (including the perfectly valid runtime
# arm) is lost. Deleting this exact form lets the runtime arm survive.
#
# We INTENTIONALLY restrict to `static_assert(IDENT())` (a simple call,
# no arguments, optional string message) — trait-style assertions like
# `static_assert(is_same_v<T,U>);` MUST stay.
_SKIP_CONSTEXPR_RE = re.compile(
    r'^[ \t]*static_assert\([ \t]*'
    r'([A-Za-z_][A-Za-z_0-9]*)'              # IDENT
    r'\([ \t]*\)'                            # ()
    r'[ \t]*(?:,[ \t]*"[^"\\]*(?:\\.[^"\\]*)*"[ \t]*)?'  # optional , "msg"
    r'\)[ \t]*;[ \t]*\r?\n',
    re.MULTILINE)


def _skip_constexpr_enabled() -> bool:
    return os.environ.get("LIBCIS_SKIP_CONSTEXPR", "") not in ("", "0")


def _strip_constexpr_asserts(text: str):
    """Delete file-scope `static_assert(IDENT());` lines. Returns
    (new_text, count). Idempotent."""
    n = 0
    def sub(_m):
        nonlocal n
        n += 1
        return ""
    new = _SKIP_CONSTEXPR_RE.sub(sub, text)
    return new, n


STRUCTURAL_ERRORS = ("file not found",)


def hard_errors(tu):
    return [d for d in tu.diagnostics if d.severity >= ci.Diagnostic.Error]


def verify(out_text: str, ref_path: str, allowed):
    """Re-parse the rewritten source: every hard diagnostic must already have
    been present in the input (same spelling) -- the rewrite may fix things,
    never break them."""
    idx = ci.Index.create()
    tu = idx.parse(ref_path, args=parse_args(), unsaved_files=[(ref_path, out_text)])
    budget = {}
    for d in allowed:
        budget[d.spelling] = budget.get(d.spelling, 0) + 1
    new = []
    for d in hard_errors(tu):
        if budget.get(d.spelling, 0) > 0:
            budget[d.spelling] -= 1
        else:
            new.append(d)
    return new


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


def process_one(args):
    # Bind libclang before the first parse.  cmd_worker does this too, but the
    # batched path (transfer_batch -> run_batch_child) calls process_one directly,
    # and the llvm-project cindex.py does not auto-load a library the way the pip
    # `libclang` wheel does -- so without this the batch parse fails to dlopen.
    _ensure_libclang()
    # cursor locations are compared by absolute path; ninja edges pass src
    # relative to the build dir
    src, rel = args
    src = os.path.abspath(args[0])
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
        idx = ci.Index.create()
        tu = idx.parse(src, args=parse_args())
        hard = hard_errors(tu)
        # structural failures (missing header etc.) leave no usable AST; pure
        # semantic failures (e.g. a constexpr static_assert the OLDER installed
        # reference can't satisfy) do -- transfer those, record the noise, and
        # let the backends judge.
        structural = [d for d in hard
                      if any(s in d.spelling for s in STRUCTURAL_ERRORS)]
        if structural:
            return {"status": "error", "file": rel, "stage": "parse",
                    "diag": str(structural[0])}
        out, entry, counts = transform(text, tu, src, slug)
        skipped_ce = 0
        if _skip_constexpr_enabled():
            out, skipped_ce = _strip_constexpr_asserts(out)
        bad = verify(out, src, hard)
        if bad:
            return {"status": "error", "file": rel, "stage": "verify",
                    "diag": str(bad[0])}
    except TransformError as ex:
        return {"status": "skipped", "file": rel, "reason": f"adapt:{ex}"}
    except Exception as ex:  # pragma: no cover
        return {"status": "error", "file": rel, "stage": "exc", "diag": repr(ex)}

    kind = "run" if (entry and not rel.endswith(".compile.pass.cpp")) else "compile"
    rec = {"status": "ok", "file": rel, "slug": slug, "kind": kind,
           "entry": entry, "flags": flags, "adapted": counts,
           "soft_errors": len(hard), "_text": out}
    if skipped_ce:
        rec["skipped_constexpr_asserts"] = skipped_ce
    return rec


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


def write_rec(rec_path, rec):
    os.makedirs(os.path.dirname(rec_path), exist_ok=True)
    text = rec.pop("_text", None)
    if rec["status"] == "ok":
        dst = os.path.join(DST_ROOT, rec["file"])
        os.makedirs(os.path.dirname(dst), exist_ok=True)
        with open(dst, "w") as fh:
            fh.write(text)
    with open(rec_path, "w") as fh:
        json.dump(rec, fh)


def cmd_worker(src, rel, rec_path):
    """One ninja edge's inner work: transform a single file, write the .cpp on
    success, and ALWAYS write its record.  May segfault/hang inside libclang --
    that is why --edge runs this under `timeout` in a child process."""
    _ensure_libclang()
    write_rec(rec_path, process_one((src, rel)))


def cmd_edge(src, rel, rec_path):
    """ninja edge wrapper: ninja has no per-command timeout, so run --worker in
    a child under a wall-clock limit.  A crash (non-zero rc) or hang (timeout)
    is captured AS the record, so the edge always produces its output and the
    build keeps going.  Always exits 0."""
    import subprocess
    try:
        p = subprocess.run(
            [sys.executable, __file__, "--worker", src, rel, rec_path],
            timeout=PER_FILE_TIMEOUT, capture_output=True, text=True)
        if p.returncode == 0 and os.path.exists(rec_path):
            return
        diag = f"rc={p.returncode} {p.stderr.strip()[-160:]}"
        stage = "crash"
    except subprocess.TimeoutExpired:
        diag, stage = f"exceeded {PER_FILE_TIMEOUT}s", "timeout"
    write_rec(rec_path, {"status": "error", "file": rel, "stage": stage, "diag": diag})


def cmd_aggregate(manifest_path, rec_paths):
    """Aggregate edge: fold every per-file record into manifest.json.
    @FILE arguments are ninja response files (one path per whitespace token)."""
    expanded = []
    for p in rec_paths:
        if p.startswith("@"):
            expanded.extend(open(p[1:]).read().split())
        else:
            expanded.append(p)
    rec_paths = expanded
    m = {"transferred": [], "skipped": [], "errors": [], "subtrees": []}
    subs = set()
    for rp in rec_paths:
        try:
            r = json.load(open(rp))
        except Exception:
            continue
        subs.add(r["file"].split("/")[0])
        if r["status"] == "ok":
            m["transferred"].append({k: r[k] for k in
                                     ("file", "slug", "kind", "entry", "flags", "adapted")})
        elif r["status"] == "skipped":
            m["skipped"].append({"file": r["file"], "reason": r["reason"]})
        else:
            m["errors"].append({"file": r["file"], "stage": r["stage"], "diag": r["diag"]})
    m["subtrees"] = sorted(subs)
    with open(manifest_path, "w") as fh:
        json.dump(m, fh, indent=1)
    from collections import Counter
    print(f"manifest: transferred={len(m['transferred'])} "
          f"(adapted={sum(1 for r in m['transferred'] if r.get('adapted'))}) "
          f"skipped={len(m['skipped'])} errors={len(m['errors'])}")
    for r, c in Counter(e["stage"] for e in m["errors"]).most_common(10):
        print(f"   err[{r}] = {c}")


def main():
    # transfer.py is the WORK, invoked one edge at a time by ninja (generated by
    # tools/gen_transfer.py).  ninja owns scheduling/parallelism/incrementality/
    # keep-going; these modes are the per-edge work + the one thing ninja lacks
    # (a per-command timeout, handled by --edge).
    mode = sys.argv[1] if len(sys.argv) > 1 else ""
    if mode == "--build-pch":
        # build_pch sys.exit's with an actionable message on any failure
        # (missing headers, parse errors); no fallback — PCH is required.
        build_pch()
    elif mode == "--worker":
        cmd_worker(sys.argv[2], sys.argv[3], sys.argv[4])
    elif mode == "--edge":
        cmd_edge(sys.argv[2], sys.argv[3], sys.argv[4])
    elif mode == "--aggregate":
        cmd_aggregate(sys.argv[2], sys.argv[3:])
    else:
        sys.exit("usage: transfer.py --build-pch|--worker|--edge|--aggregate ...\n"
                 "       (generate the build with tools/gen_transfer.py)")


if __name__ == "__main__":
    main()
