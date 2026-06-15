#!/usr/bin/env python3
"""Probe the build/test toolchain and report what is present, what is missing,
and exactly which environment variable to set to fix each gap.

Run this first on any new machine:

    python3 tools/doctor.py

Exit status is 0 iff everything REQUIRED to build the library and run the
per-file gate (`$CXX` and the libc++ test corpus) is present.  The reference
backends, libclang, and the PCH headers are optional and reported as warnings.
"""
import os
import shutil
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
import config as cfg  # noqa: E402

OK, WARN, BAD = "ok  ", "WARN", "MISS"


def _cxx(cxx):
    return shutil.which(cxx.split()[0])


def main():
    rows = []          # (level, label, detail)
    required_ok = True

    # --- the one required compiler: the library backend -------------------
    p = _cxx(cfg.CXX_LIBCIS)
    if p:
        rows.append((OK, f"libcis compiler  ($CXX={cfg.CXX_LIBCIS})", p))
    else:
        required_ok = False
        rows.append((BAD, f"libcis compiler  ($CXX={cfg.CXX_LIBCIS})",
                     "not on PATH -- set CXX=<your gcc-10-compatible g++>"))

    # --- the test corpus: required to (re)run the transfer ----------------
    try:
        d = cfg.libcxx_test_dir()
        rows.append((OK, "libc++ test corpus ($LIBCXX)", d))
    except SystemExit:
        required_ok = False
        rows.append((BAD, "libc++ test corpus ($LIBCXX)",
                     "not found -- clone llvm-project, set LIBCXX=.../libcxx/test"))

    # --- optional reference / discriminator backends ----------------------
    for label, cxx, env in (
        ("libcxx backend",    cfg.CXX_LIBCXX,    "CXX_LIBCXX"),
        ("libstdcxx backend", cfg.CXX_LIBSTDCXX, "CXX_LIBSTDCXX"),
        ("gcc10std backend",  cfg.CXX_GCC10STD,  "CXX_GCC10STD"),
    ):
        p = _cxx(cxx)
        if p:
            rows.append((OK, f"{label}  (${env}={cxx})", p))
        else:
            rows.append((WARN, f"{label}  (${env}={cxx})",
                         "absent (optional) -- discriminator/reference runs disabled"))

    # --- libclang + bindings: only needed to RE-RUN the transfer ----------
    try:
        import clang.cindex  # noqa: F401
        bindings = True
    except Exception as e:
        bindings = False
        rows.append((WARN, "python libclang bindings",
                     f"missing ({e}); `pip install libclang` to re-run transfer"))
    if bindings:
        try:
            rows.append((OK, "libclang ($LIBCLANG)", cfg.find_libclang()))
        except SystemExit:
            rows.append((WARN, "libclang ($LIBCLANG)",
                         "not found -- set LIBCLANG=/path/to/libclang.so to re-run transfer"))

    # --- clang builtin headers: needed by every transfer parse ------------
    if bindings:
        b = cfg.clang_builtin_include_dir()
        rows.append((OK if b else WARN, "clang builtins ($CLANG_BUILTIN_INCLUDE)",
                     b or "not found -- transfer parses will fail on <stddef.h>; "
                          "set CLANG_BUILTIN_INCLUDE=<clang resource-dir>/include"))

    # --- PCH headers: pure optimization for the transfer ------------------
    inc = cfg.libcxx_include_dir()
    rows.append((OK if inc else WARN, "PCH headers ($LIBCXX_INCLUDE)",
                 inc or "absent (optional) -- transfer parses without a PCH"))

    # --- ninja: drives the transfer + group build -------------------------
    nj = shutil.which("ninja")
    rows.append((OK if nj else WARN, "ninja",
                 nj or "absent -- needed for the transfer/group build graph"))

    w = max(len(label) for _, label, _ in rows)
    print("libcis toolchain check")
    print("=" * (w + 30))
    for level, label, detail in rows:
        print(f"[{level}] {label.ljust(w)}  {detail}")
    print("=" * (w + 30))
    if required_ok:
        print("REQUIRED toolchain present (library + per-file gate can run).")
    else:
        print("MISSING required toolchain -- see [MISS] rows above.")
    return 0 if required_ok else 1


if __name__ == "__main__":
    sys.exit(main())
