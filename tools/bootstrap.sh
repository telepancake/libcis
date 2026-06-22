#!/usr/bin/env bash
# Download the pinned toolchain the test pipeline needs into ./toolchain, so the
# build does not depend on which compiler/header VERSIONS the system happens to
# have, or where they live.  This is deliberately NOT "bazel hermetic": the
# fetched compilers still run against the host's glibc + binutils (as/ld).  What
# it pins locally is the version-sensitive parts:
#
#   toolchain/root/        g++-10, clang-20 (libclang/libLLVM), libc++-20 headers
#                          and clang's resource headers -- all extracted Ubuntu
#                          .debs.  -> $CXX, $LIBCLANG, $LIBCXX_INCLUDE, resource
#   toolchain/llvm-project/ the libc++ test corpus + clang's python bindings
#                          (cindex.py), one sparse git checkout at a matching tag
#
# clang and the corpus are pinned to the SAME LLVM release: a corpus newer than
# the parser does not parse (newer test_macros.h / headers use newer clang), and
# the cindex.py bindings must match the libclang they drive.  We use clang-20 (=
# the repo's reference) rather than the pip `libclang` wheel, which tops out at
# 18.1 and would drag in an older, mismatched corpus.
#
# tools/config.py prefers all of the above automatically once present, so after
# `make bootstrap` the rest of the pipeline (make smoke/support/gate/test) just
# works.  Re-running this script is cheap: each step is skipped when already done.
#
# Requirements on the host: bash, git, dpkg-deb, and apt-get (a Debian/Ubuntu
# host, for the .deb fetch).  No pip/uv/conda needed.
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
TC="$ROOT/toolchain"

# clang and the corpus are pinned together to the 20.1 line.  The .deb clang on
# Ubuntu noble is 20.1.2; the checkout (corpus + cindex.py) is a 20.1.x tag.
LLVM_TAG="llvmorg-20.1.8"

# The .debs to extract locally:
#   - gcc-10 core + the support libs cpp-10 needs (isl/mpc/mpfr/gmp), so it runs
#     on a host that lacks them;
#   - clang-20: libclang1-20 (the C library the parse uses) + libllvm20 (its
#     backing libLLVM) + libclang-common-20-dev (clang's resource/builtin headers);
#   - libc++-20 dev headers -- a COMPLETE set including the CMake-generated
#     __config_site / __assertion_handler a raw source checkout lacks, so the
#     parse finds libc++ without any system libc++.
# Everything else (glibc, binutils) comes from the host.
DEBS=(g++-10 gcc-10 cpp-10 gcc-10-base libgcc-10-dev libstdc++-10-dev
      libisl23 libmpc3 libmpfr6 libgmp10
      libclang1-20 libllvm20 libclang-common-20-dev
      libc++-20-dev libc++abi-20-dev)

say() { printf '\033[36m[bootstrap]\033[0m %s\n' "$*"; }

# --- compilers + headers: download + extract Ubuntu .debs into toolchain/root
fetch_debs() {
    if [ -x "$TC/root/usr/bin/g++-10" ] \
       && compgen -G "$TC/root/usr/lib/llvm-*/include/c++/v1"   >/dev/null \
       && compgen -G "$TC/root/usr/lib/*/libclang-*.so.*"       >/dev/null; then
        say "compilers + headers already present, skipping"
        return
    fi
    command -v apt-get >/dev/null || {
        echo "bootstrap: apt-get not found; cannot fetch the .debs on this host." >&2
        echo "  Run on a Debian/Ubuntu host, or install g++-10 + clang-20 +" >&2
        echo "  libc++-20-dev yourself and point \$CXX/\$LIBCLANG/etc at them." >&2
        exit 1
    }
    say "downloading .debs (gcc-10, clang-20, libc++-20)"
    mkdir -p "$TC/debs" "$TC/root"
    ( cd "$TC/debs" && apt-get download "${DEBS[@]}" )
    say "extracting into toolchain/root"
    for d in "$TC"/debs/*.deb; do dpkg-deb -x "$d" "$TC/root"; done
    "$TC/root/usr/bin/g++-10" --version | head -1
}

# --- corpus + clang python bindings: one sparse llvm-project checkout --------
fetch_corpus() {
    if [ -d "$TC/llvm-project/libcxx/test/std" ] \
       && [ -f "$TC/llvm-project/clang/bindings/python/clang/cindex.py" ]; then
        say "llvm-project checkout already present, skipping"
        return
    fi
    say "cloning llvm-project @ $LLVM_TAG (sparse, blobless)"
    rm -rf "$TC/llvm-project"
    git clone --depth 1 --filter=blob:none --sparse --branch "$LLVM_TAG" \
        https://github.com/llvm/llvm-project "$TC/llvm-project"
    git -C "$TC/llvm-project" sparse-checkout set \
        libcxx/test clang/bindings/python
}

mkdir -p "$TC"
fetch_debs
fetch_corpus
touch "$TC/.bootstrap-ok"
say "done — toolchain ready under ./toolchain"
