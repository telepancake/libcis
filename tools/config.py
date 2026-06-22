#!/usr/bin/env python3
"""Central toolchain/path configuration for the libcis transfer + test pipeline.

Everything that used to be hardcoded to one specific build container lives here
and is overridable from the environment, so the pipeline is not pinned to a
single machine.  The defaults reproduce the original reference toolchain
(gcc-10 library backend; clang-20 / gcc-14 reference backends; an LLVM monorepo
checkout as the test corpus).  When you have something else, set the matching
environment variable and nothing else needs to change.

Override knobs (all optional; shown with their defaults)
--------------------------------------------------------
  CXX             g++-10       library / `libcis` backend compiler (the one
                               supported compiler for the library itself)
  CXX_LIBCXX      clang++-20   `libcxx` reference-backend compiler
  CXX_LIBSTDCXX   g++-14       `libstdcxx` reference-backend compiler
  CXX_GCC10STD    = $CXX       `gcc10std` discriminator compiler
  LIBCLANG        (autodetect) libclang shared object for tools/transfer.py
  LIBCXX          (autodetect) libc++ `test` directory: the source test corpus
  LIBCXX_INCLUDE  (autodetect) libc++ header directory used to build the
                               transfer PCH

`libcis` itself only requires `$CXX` (gcc-10) and the test corpus.  The
reference backends and libclang are optional and only consulted by the tools
that actually use them, so a machine missing clang/gcc-14 can still build and
gate the library.
"""
import glob
import os
import shutil
import subprocess

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# ---------------------------------------------------------------------------
# Local bootstrapped toolchain (tools/bootstrap.sh populates ./toolchain).
# When present, every resolver below prefers it, so the pipeline needs nothing
# from the system beyond glibc + binutils.  An explicit environment variable
# still wins over the local toolchain, which still wins over system probing.
# ---------------------------------------------------------------------------
TOOLCHAIN_DIR     = os.path.join(ROOT, "toolchain")
# g++-10 is built from source into toolchain/gcc (no portable prebuilt exists).
_LOCAL_GXX        = os.path.join(TOOLCHAIN_DIR, "gcc", "bin", "g++-10")
# libclang comes from the pip/uv `libclang` wheel (a per-arch, self-contained
# shared object + the cindex.py bindings) unpacked into toolchain/pylibs.
PYLIBS_DIR        = os.path.join(TOOLCHAIN_DIR, "pylibs")
_LOCAL_LIBCLANG   = os.path.join(PYLIBS_DIR, "clang", "native", "libclang.so")
# The corpus, the libc++ headers, and clang's builtin headers are arch-agnostic
# source from one llvm-project checkout (libc++'s two generated headers are
# produced by tools/bootstrap.sh).
_LOCAL_LLVM       = os.path.join(TOOLCHAIN_DIR, "llvm-project")
_LOCAL_LIBCXX     = os.path.join(_LOCAL_LLVM, "libcxx", "test")
_LOCAL_LIBCXX_INC = os.path.join(_LOCAL_LLVM, "libcxx", "include")
_LOCAL_RESOURCE   = os.path.join(TOOLCHAIN_DIR, "resource")  # include/ -> clang Headers

# ---------------------------------------------------------------------------
# Compilers.  These are plain strings (no PATH probing at import time); call
# require_cxx() at the point of use to fail loudly with guidance when missing.
# ---------------------------------------------------------------------------
def _default_cxx():
    """$CXX if set, else the local toolchain's g++-10, else bare `g++-10`."""
    if os.environ.get("CXX"):
        return os.environ["CXX"]
    if os.path.exists(_LOCAL_GXX):
        return _LOCAL_GXX
    return "g++-10"


CXX_LIBCIS    = _default_cxx()
CXX_LIBCXX    = os.environ.get("CXX_LIBCXX",     "clang++-20")
CXX_LIBSTDCXX = os.environ.get("CXX_LIBSTDCXX",  "g++-14")
CXX_GCC10STD  = os.environ.get("CXX_GCC10STD",   CXX_LIBCIS)

# Mapping consumed by the multi-backend tools (gen_groups.py, gen_ninja.py,
# measure_std.py, run_std.sh).  Keep the keys in sync with those tools.
BACKEND_CXX = {
    "libcis":    CXX_LIBCIS,
    "libcxx":    CXX_LIBCXX,
    "libstdcxx": CXX_LIBSTDCXX,
    "gcc10std":  CXX_GCC10STD,
}

# The matching environment variable, for error messages.
_BACKEND_ENV = {
    "libcis": "CXX", "libcxx": "CXX_LIBCXX",
    "libstdcxx": "CXX_LIBSTDCXX", "gcc10std": "CXX_GCC10STD",
}


def require_cxx(cxx, backend=None):
    """Return `cxx` if its program is on PATH, else raise SystemExit with the
    environment variable to set to point at a compiler you do have."""
    prog = cxx.split()[0]
    if shutil.which(prog) is not None:
        return cxx
    env = _BACKEND_ENV.get(backend, "CXX")
    raise SystemExit(
        f"compiler {prog!r} not found on PATH.\n"
        f"  Install it, or set {env} to a compiler you have, e.g.\n"
        f"    {env}=g++-13 {prog and ''}<command>")


# ---------------------------------------------------------------------------
# libclang shared object (only tools/transfer.py needs this).
# ---------------------------------------------------------------------------
def _libclang_candidates():
    pats = [
        "/usr/lib/llvm-*/lib/libclang-*.so*",
        "/usr/lib/llvm-*/lib/libclang.so*",
        "/usr/lib/*/libclang-*.so*",
        "/lib/*/libclang-*.so*",
        "/usr/lib/libclang.so*",
    ]
    found = []
    for p in pats:
        found += glob.glob(p)
    # `libclang-cpp.so` is the C++ API library; the cindex bindings need the C
    # `libclang.so`/`libclang-NN.so`, so drop the C++ one.
    found = [f for f in found if "libclang-cpp" not in os.path.basename(f)]
    # Highest version first (libclang-20 before libclang-18); stable otherwise.
    return sorted(set(found), reverse=True)


def find_libclang():
    """Path to a libclang shared object, from $LIBCLANG or autodetection."""
    env = os.environ.get("LIBCLANG")
    if env:
        if not os.path.exists(env):
            raise SystemExit(f"LIBCLANG={env!r} does not exist")
        return env
    if os.path.exists(_LOCAL_LIBCLANG):
        return _LOCAL_LIBCLANG
    for c in _libclang_candidates():
        if os.path.exists(c):
            return c
    raise SystemExit(
        "libclang shared library not found.\n"
        "  Install it (e.g. `apt-get install libclang-20-dev` / `clang-20`)\n"
        "  or set LIBCLANG=/path/to/libclang.so")


# ---------------------------------------------------------------------------
# libc++ test corpus: the directory tools/transfer.py rewrites tests from.
# ---------------------------------------------------------------------------
def _libcxx_test_candidates():
    return [
        _LOCAL_LIBCXX,
        os.path.join(ROOT, "third_party", "llvm-project", "libcxx", "test"),
        os.path.expanduser("~/llvm-project/libcxx/test"),
        "/home/user/llvm-project/libcxx/test",
    ]


def libcxx_test_dir(required=True):
    """The libc++ `test` directory (contains `std/` and `support/`).

    Honors $LIBCXX; otherwise probes a few conventional checkout locations.
    Raises SystemExit with clone instructions when `required` and not found.
    """
    env = os.environ.get("LIBCXX")
    cands = [env] if env else _libcxx_test_candidates()
    for c in cands:
        if c and os.path.isdir(os.path.join(c, "std")):
            return c
    if not required:
        return env or _libcxx_test_candidates()[0]
    raise SystemExit(
        "libc++ test corpus not found"
        + (f" at LIBCXX={env!r}" if env else "") + ".\n"
        "  Clone the LLVM monorepo and point LIBCXX at libcxx/test, e.g.\n"
        "    git clone --depth 1 https://github.com/llvm/llvm-project ~/llvm-project\n"
        "    export LIBCXX=~/llvm-project/libcxx/test")


# ---------------------------------------------------------------------------
# libc++ header directory used to build the transfer PCH (optimization only).
# ---------------------------------------------------------------------------
def _libcxx_include_candidates():
    return [_LOCAL_LIBCXX_INC] \
        + sorted(glob.glob("/usr/lib/llvm-*/include/c++/v1"), reverse=True) \
        + ["/usr/include/c++/v1"]


def libcxx_include_dir():
    """libc++ header dir for the PCH, or None (PCH is optional; transfer falls
    back to parsing without it)."""
    env = os.environ.get("LIBCXX_INCLUDE")
    cands = [env] if env else _libcxx_include_candidates()
    for c in cands:
        if c and os.path.isdir(c):
            return c
    return None


# Where the prebuilt transfer PCH lands.
PCH_PATH = os.path.join(ROOT, "build", "transfer.pch")


# ---------------------------------------------------------------------------
# Clang "resource dir": where clang's builtin headers (stddef.h, stdarg.h, ...)
# live.  libclang does not always compute this correctly for itself, so the
# transfer parse dies either with "'stddef.h' file not found" (resource include
# missing entirely) or, if we naively force it onto the path with -isystem, with
# libc++'s "<cstddef> didn't find libc++'s <stddef.h>" (builtin shadowing v1).
# The correct fix is to hand the ROOT to clang via -resource-dir and let the
# frontend order it properly: libc++ headers first, then the builtins, then the
# C library -- exactly the order libc++'s #include_next dance requires.
# ---------------------------------------------------------------------------
def _has_stddef(d):
    return bool(d) and os.path.isfile(os.path.join(d, "stddef.h"))


def clang_resource_dir():
    """Clang resource-dir ROOT (the dir whose `include/` holds stddef.h), or
    None.  Pass it to libclang via -resource-dir.  Honors $CLANG_RESOURCE_DIR;
    otherwise asks an available clang, then globs the usual install layouts."""
    env = os.environ.get("CLANG_RESOURCE_DIR")
    if env:
        return env if _has_stddef(os.path.join(env, "include")) else None

    # The local toolchain ships clang's builtin headers via a symlink under
    # toolchain/resource/include -> the llvm-project checkout's clang/lib/Headers.
    if _has_stddef(os.path.join(_LOCAL_RESOURCE, "include")):
        return _LOCAL_RESOURCE

    # Ask a clang binary where its resource dir is (most reliable).
    for c in (CXX_LIBCXX.split()[0], "clang++", "clang"):
        if shutil.which(c) is None:
            continue
        try:
            rd = subprocess.run([c, "-print-resource-dir"],
                                capture_output=True, text=True,
                                timeout=10).stdout.strip()
        except Exception:
            continue
        if _has_stddef(os.path.join(rd, "include")):
            return rd

    # Glob the conventional install layouts; prefer the highest version.  The
    # root is the parent of the include/ dir.
    cands = glob.glob("/usr/lib/llvm-*/lib/clang/*/include") \
        + glob.glob("/usr/lib/clang/*/include")
    for inc in sorted(set(cands), reverse=True):
        if _has_stddef(inc):
            return os.path.dirname(inc)
    return None


def clang_builtin_include_dir():
    """The `include/` under the resource dir (for display/diagnostics)."""
    rd = clang_resource_dir()
    return os.path.join(rd, "include") if rd else None
