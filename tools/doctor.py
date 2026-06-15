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
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
import config as cfg  # noqa: E402

OK, WARN, BAD = "ok  ", "WARN", "MISS"

# The libcis backend flags + link tail, kept in sync with tools/gen_groups.py
# and the README.  Used by the smoke build below.
_SMOKE_FLAGS = ("-std=gnu++20 -fcoroutines -fno-exceptions -fno-rtti "
                "-nostdinc++ -Iinclude -O0 -w").split()
_SMOKE_LINK = "-nodefaultlibs -lpthread -lm -lc -lgcc_s -lgcc".split()
_SMOKE_SRC = ("#include <vector>\n#include <algorithm>\n"
              "int main(){std::vector<int> v{3,1,2};"
              "std::sort(v.begin(),v.end());return v[0]!=1;}\n")


def _cxx(cxx):
    return shutil.which(cxx.split()[0])


def _smoke_build():
    """Actually compile+link+run a trivial program against the library, so the
    doctor reports the real build state instead of mere tool presence.  Returns
    (ok, detail)."""
    with tempfile.TemporaryDirectory() as td:
        src = os.path.join(td, "smoke.cpp")
        exe = os.path.join(td, "smoke")
        with open(src, "w") as fh:
            fh.write(_SMOKE_SRC)
        cmd = (cfg.CXX_LIBCIS.split() + _SMOKE_FLAGS
               + [src, os.path.join(ROOT, "src", "support.cpp")]
               + _SMOKE_LINK + ["-o", exe])
        try:
            p = subprocess.run(cmd, cwd=ROOT, capture_output=True,
                               text=True, timeout=180)
        except Exception as e:
            return False, f"compile failed to launch: {e}"
        if p.returncode != 0:
            err = next((l for l in p.stderr.splitlines() if "error:" in l),
                       p.stderr.strip().splitlines()[-1] if p.stderr.strip()
                       else "non-zero exit, no diagnostic")
            return False, "compile FAILED: " + err[:90]
        try:
            r = subprocess.run([exe], capture_output=True, timeout=60)
        except Exception as e:
            return False, f"built but failed to run: {e}"
        if r.returncode != 0:
            return False, f"built but exited {r.returncode} (runtime broken)"
        return True, "compiles + links + runs"


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

    # --- the real check: does the library actually BUILD? -----------------
    # Presence of a compiler is not the same as a working library, so smoke
    # compile+link+run a trivial TU against include/ + src/support.cpp.  This
    # is what makes the doctor's verdict mean "you can build", not "tools exist".
    if p:
        ok, detail = _smoke_build()
        if ok:
            rows.append((OK, "library smoke build", detail))
        else:
            required_ok = False
            rows.append((BAD, "library smoke build", detail))

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

    # --- clang resource dir: needed by every transfer parse ---------------
    if bindings:
        rd = cfg.clang_resource_dir()
        rows.append((OK if rd else WARN, "clang resource dir ($CLANG_RESOURCE_DIR)",
                     (rd + "/include") if rd else
                     "not found -- transfer parses will fail on <stddef.h>; "
                     "set CLANG_RESOURCE_DIR=<clang -print-resource-dir>"))

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
