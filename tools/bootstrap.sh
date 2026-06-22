#!/usr/bin/env bash
# Provision the toolchain the test pipeline needs into ./toolchain WITHOUT using
# the host's package manager or any prebuilt, distro/arch-specific binaries, so
# the build does not depend on what the host has installed or where.  Three
# pieces, each from a portable source:
#
#   toolchain/gcc/         g++-10, BUILT FROM the GNU source release.  No portable
#                          prebuilt gcc exists, so source is the only arch-neutral
#                          way; configure detects the host arch.   -> $CXX
#   toolchain/pylibs/      libclang -- the `libclang` wheel from PyPI via uv/pip.
#                          Wheels are per-arch and self-contained, so pip resolves
#                          the host CPU for us and nothing else is needed.  -> $LIBCLANG
#   toolchain/llvm-project/ the libc++ test corpus, the libc++ headers, clang's
#                          builtin headers, and clang's cindex.py -- one sparse
#                          git checkout (arch-independent source).  libc++'s two
#                          CMake-generated headers are generated here from their
#                          .in templates (what configure_file would do).
#
# Pins: the libclang wheel and the checkout are the same LLVM release line so the
# parse agrees with the corpus.  tools/config.py prefers all of the above once
# present, so `make smoke/support/gate/test` just work after this.  Each step is
# skipped when already done; `make distclean` forces a redo.
#
# Host needs: bash, git, curl, tar, make, a C/C++ compiler (to build gcc), and
# uv or pip.  No apt/dpkg/conda, no architecture assumptions.
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
TC="$ROOT/toolchain"

GCC_VER=10.5.0
LLVM_TAG=llvmorg-18.1.8
LIBCLANG_VERSION=18.1.1   # newest `libclang` on PyPI; pins the checkout's line

say() { printf '\033[36m[bootstrap]\033[0m %s\n' "$*"; }

# --- g++-10: build from the GNU source release into toolchain/gcc -----------
build_gcc() {
    if [ -x "$TC/gcc/bin/g++-10" ]; then say "g++-10 already built, skipping"; return; fi
    command -v make >/dev/null && command -v g++ >/dev/null || {
        echo "bootstrap: need make + a host C/C++ compiler to build gcc." >&2; exit 1; }
    say "building gcc-$GCC_VER from source (this takes a while)"
    mkdir -p "$TC/src"; cd "$TC/src"
    [ -f gcc-$GCC_VER.tar.xz ] || curl -fLO "https://ftp.gnu.org/gnu/gcc/gcc-$GCC_VER/gcc-$GCC_VER.tar.xz"
    [ -d gcc-$GCC_VER ] || tar xf gcc-$GCC_VER.tar.xz
    # gmp/mpfr/mpc/isl SOURCE into the tree (built in-tree; no system libs)
    [ -d gcc-$GCC_VER/gmp ] || ( cd gcc-$GCC_VER && ./contrib/download_prerequisites )
    rm -rf gcc-build && mkdir gcc-build && cd gcc-build
    # --disable-libsanitizer: gcc-10's bundled libsanitizer does not build against
    # glibc >= 2.34 (it uses `struct termio`, dropped from modern <sys/ioctl.h>).
    # libcis never uses sanitizers, so skip that runtime library entirely.
    "$TC/src/gcc-$GCC_VER/configure" --prefix="$TC/gcc" \
        --enable-languages=c,c++ --disable-multilib --disable-bootstrap \
        --disable-werror --disable-libsanitizer --program-suffix=-10
    make -j"$(nproc)"
    make install
    rm -rf "$TC/src/gcc-build"            # reclaim the build tree
    "$TC/gcc/bin/g++-10" --version | head -1
}

# --- libclang: the per-arch, self-contained wheel into toolchain/pylibs -----
fetch_libclang() {
    if [ -f "$TC/pylibs/clang/native/libclang.so" ]; then
        say "libclang already present, skipping"; return; fi
    say "installing libclang==$LIBCLANG_VERSION (per-arch wheel) into toolchain/pylibs"
    if command -v uv >/dev/null; then
        uv pip install --target "$TC/pylibs" --python python3 "libclang==$LIBCLANG_VERSION"
    elif command -v pip >/dev/null; then
        pip install --target "$TC/pylibs" "libclang==$LIBCLANG_VERSION"
    else
        echo "bootstrap: need uv or pip to fetch the libclang wheel." >&2; exit 1
    fi
}

# --- corpus + libc++/clang headers + cindex: one sparse llvm checkout -------
fetch_sources() {
    if [ ! -d "$TC/llvm-project/libcxx/test/std" ]; then
        say "cloning llvm-project @ $LLVM_TAG (sparse, blobless)"
        rm -rf "$TC/llvm-project"
        git clone --depth 1 --filter=blob:none --sparse --branch "$LLVM_TAG" \
            https://github.com/llvm/llvm-project "$TC/llvm-project"
        git -C "$TC/llvm-project" sparse-checkout set \
            libcxx/include libcxx/test libcxx/vendor clang/lib/Headers
    else
        say "llvm-project checkout already present, skipping clone"
    fi
    # Generate libc++'s two CMake-generated headers (what configure_file does for
    # a default, hardening=none, non-Apple build) -- so no prebuilt libc++ needed.
    say "generating libc++ __config_site + __assertion_handler"
    python3 "$ROOT/tools/gen_libcxx_site.py" "$TC/llvm-project/libcxx/include"
    # clang -resource-dir wants a ROOT whose include/ has stddef.h.
    mkdir -p "$TC/resource"
    ln -sfn ../llvm-project/clang/lib/Headers "$TC/resource/include"
}

mkdir -p "$TC"
build_gcc
fetch_libclang
fetch_sources
touch "$TC/.bootstrap-ok"
say "done — toolchain ready under ./toolchain"
