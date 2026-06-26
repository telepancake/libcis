#!/usr/bin/env bash
# Provision the toolchain the test pipeline needs into ./toolchain WITHOUT using
# the host's package manager or any prebuilt, distro/arch-specific binary.  Both
# compilers are BUILT FROM SOURCE, so they adapt to the host (arch and C library)
# instead of assuming one:
#
#   toolchain/gcc/    g++-10, built from the GNU source release.   -> $CXX
#   toolchain/llvm/   clang-20 libclang + libc++ headers + clang's resource
#                     headers, built from the llvm-project source.  libc++ is
#                     cmake-CONFIGURED for the host's C library, so its headers
#                     match whatever libc the host has (glibc, musl, ...).
#   toolchain/llvm-project/  the source checkout: the libc++ test corpus and the
#                     matching clang cindex.py bindings.
#
# clang and the corpus are the same LLVM release, so the parser agrees with the
# tests.  tools/config.py prefers all of the above once present, so `make
# smoke/support/gate/test` just work after this.  Each step is skipped when
# already done; `make distclean` forces a redo.
#
# Host needs: bash, git, curl, tar, cmake, ninja, make, and a C/C++ compiler (to
# build gcc + llvm).  No apt/dpkg/conda, no pip, no architecture assumptions.
# Building both compilers takes a while on the first run; everything is cached.
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
TC="$ROOT/toolchain"

GCC_VER=10.5.0
LLVM_TAG=llvmorg-20.1.8

say() { printf '\033[36m[bootstrap]\033[0m %s\n' "$*"; }

# --- the llvm-project source: built into clang/libc++ AND the test corpus -----
fetch_sources() {
    if [ -d "$TC/llvm-project/.git" ]; then say "llvm-project checkout present, skipping"; return; fi
    say "cloning llvm-project @ $LLVM_TAG (full tree, blobless)"
    rm -rf "$TC/llvm-project"
    git clone --depth 1 --filter=blob:none --branch "$LLVM_TAG" \
        https://github.com/llvm/llvm-project "$TC/llvm-project"
}

# --- g++-10: build from the GNU source release into toolchain/gcc ------------
build_gcc() {
    if [ -x "$TC/gcc/bin/g++-10" ]; then say "g++-10 already built, skipping"; return; fi
    say "building gcc-$GCC_VER from source (this takes a while)"
    mkdir -p "$TC/src"; cd "$TC/src"
    [ -f gcc-$GCC_VER.tar.xz ] || curl -fLO "https://ftp.gnu.org/gnu/gcc/gcc-$GCC_VER/gcc-$GCC_VER.tar.xz"
    [ -d gcc-$GCC_VER ] || tar xf gcc-$GCC_VER.tar.xz
    [ -d gcc-$GCC_VER/gmp ] || ( cd gcc-$GCC_VER && ./contrib/download_prerequisites )
    rm -rf gcc-build && mkdir gcc-build && cd gcc-build
    # --disable-libsanitizer: gcc-10's bundled libsanitizer does not build against
    # glibc >= 2.34 (it uses `struct termio`, dropped from modern <sys/ioctl.h>);
    # libcis never uses sanitizers, so skip that runtime library entirely.
    "$TC/src/gcc-$GCC_VER/configure" --prefix="$TC/gcc" \
        --enable-languages=c,c++ --disable-multilib --disable-bootstrap \
        --disable-werror --disable-libsanitizer --program-suffix=-10
    make -j"$(nproc)"
    make install
    rm -rf "$TC/src/gcc-build" "$TC/src/gcc-$GCC_VER"   # reclaim space (keep tarball)
    "$TC/gcc/bin/g++-10" --version | head -1
}

# --- clang-20 libclang + libc++ headers + resource headers, from source ------
build_llvm() {
    if compgen -G "$TC"/llvm/lib/libclang.so* >/dev/null && \
       [ -d "$TC/llvm/include/c++/v1" ]; then say "llvm already built, skipping"; return; fi
    command -v cmake >/dev/null && command -v ninja >/dev/null || {
        echo "bootstrap: need cmake + ninja to build llvm." >&2; exit 1; }

    # 1. libclang (+ clang's builtin/resource headers) from the clang project.
    #    Shared libLLVM keeps the build/footprint down; Native target only.
    say "building clang/libclang from source (this takes a while)"
    cmake -G Ninja -S "$TC/llvm-project/llvm" -B "$TC/llvm-build" \
        -DCMAKE_BUILD_TYPE=Release \
        -DLLVM_ENABLE_PROJECTS=clang \
        -DLLVM_TARGETS_TO_BUILD=Native \
        -DLLVM_BUILD_LLVM_DYLIB=ON -DLLVM_LINK_LLVM_DYLIB=ON \
        -DLLVM_INCLUDE_TESTS=OFF -DLLVM_INCLUDE_EXAMPLES=OFF \
        -DLLVM_INCLUDE_BENCHMARKS=OFF -DCLANG_INCLUDE_TESTS=OFF \
        -DCMAKE_INSTALL_PREFIX="$TC/llvm"
    ninja -C "$TC/llvm-build" libclang
    # install-LLVM too: this is a shared-libLLVM build, so libclang.so needs
    # libLLVM.so beside it (install-libclang alone does not bring it).
    ninja -C "$TC/llvm-build" \
        install-libclang install-LLVM install-clang-resource-headers

    # 2. libc++ HEADERS only, configured by cmake for the host's libc.  Disabling
    #    the libc++ library means no compilation (and no clang dependency) -- just
    #    the host-correct header tree, including the generated __config_site.
    say "generating + installing host-configured libc++ headers"
    cmake -G Ninja -S "$TC/llvm-project/runtimes" -B "$TC/llvm-cxxhdr" \
        -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_RUNTIMES=libcxx \
        -DLIBCXX_ENABLE_SHARED=OFF -DLIBCXX_ENABLE_STATIC=OFF \
        -DCMAKE_INSTALL_PREFIX="$TC/llvm"
    ninja -C "$TC/llvm-cxxhdr" install-cxx-headers

    rm -rf "$TC/llvm-build" "$TC/llvm-cxxhdr"   # reclaim build trees (keep install)
    ls "$TC"/llvm/lib/libclang.so*
}

# Two scopes.  The library + the COMMITTED tests only need g++-10, so that is
# the default (`bootstrap.sh` / `make bootstrap`).  The transfer additionally
# needs clang/libclang + the libc++ corpus, built only on demand
# (`bootstrap.sh transfer` / `make bootstrap-transfer`) -- the expensive half
# the committed test/std/ lets the common case skip entirely.
MODE="${1:-gcc}"

mkdir -p "$TC"
build_gcc
touch "$TC/.gcc-ok"
case "$MODE" in
    gcc)
        say "done — library/test toolchain (g++-10) ready under ./toolchain"
        say "  (run 'tools/bootstrap.sh transfer' to add the transfer toolchain)" ;;
    transfer|full)
        fetch_sources
        build_llvm
        touch "$TC/.bootstrap-ok"
        say "done — full toolchain (g++-10 + transfer) ready under ./toolchain" ;;
    *)
        echo "usage: tools/bootstrap.sh [gcc|transfer]" >&2; exit 2 ;;
esac
