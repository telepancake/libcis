#!/usr/bin/env bash
# Build and run the libcis test suite against MULTIPLE standard libraries, so a
# test is only trusted if it passes against real vendor libraries too.
#
# Backends:
#   libcis     - our library      (g++-10, gnu++20, -nostdinc++ -Iinclude, freestanding via src/support.cpp)
#   libcxx     - vendor LLVM libc++ (clang++-18, gnu++23, -stdlib=libc++)
#   libstdcxx  - vendor GNU libstdc++ (g++-13, gnu++23)
#
# All backends use -fno-exceptions -fno-rtti so the library is the only variable.
# Each test file is compiled+linked+run on its own; we tally COMPILE and RUN
# (pass = exit 0; a failed CHECK traps -> non-zero).
#
# Usage: ./check.sh [libcis|libcxx|libstdcxx|all]   (default: all)
set -uo pipefail
cd "$(dirname "$0")"
mkdir -p build/check

want="${1:-all}"
common="-fno-exceptions -fno-rtti -Itest -O0 -w"

run_backend() {
  local name="$1"; shift
  local compile_cmd="$1"; shift   # a format string with @SRC@ @MAIN@ @OUT@
  local cpass=0 cfail=0 rpass=0 rfail=0
  local cfails=() rfails=()
  for f in test/*.cpp; do
    local base; base="$(basename "${f%.cpp}")"
    local main="build/check/${base}.main.cpp"
    local out="build/check/${base}.${name}"
    tools/gen_main.sh "$main" "$f" 2>/dev/null
    local cmd="${compile_cmd//@SRC@/$f}"; cmd="${cmd//@MAIN@/$main}"; cmd="${cmd//@OUT@/$out}"
    if eval "$cmd" >/dev/null 2>&1; then
      cpass=$((cpass+1))
      if "./$out" >/dev/null 2>&1; then rpass=$((rpass+1)); else rfail=$((rfail+1)); rfails+=("$base"); fi
    else
      cfail=$((cfail+1)); cfails+=("$base")
    fi
  done
  local total=$((cpass+cfail))
  printf "%-11s compile %2d/%-2d   run-pass %2d/%-2d\n" "$name" "$cpass" "$total" "$rpass" "$cpass"
  [ ${#cfails[@]} -gt 0 ] && printf "   compile-fail: %s\n" "${cfails[*]}"
  [ ${#rfails[@]} -gt 0 ] && printf "   run-fail:     %s\n" "${rfails[*]}"
}

echo "=== libcis test matrix across standard libraries ==="
if [ "$want" = all ] || [ "$want" = libcis ]; then
  run_backend libcis "g++-10 -std=gnu++20 $common -nostdinc++ -Iinclude @SRC@ src/support.cpp @MAIN@ -nodefaultlibs -o @OUT@ -lpthread -lm -lc -lgcc_s -lgcc"
fi
if [ "$want" = all ] || [ "$want" = libcxx ]; then
  run_backend libcxx "clang++-18 -std=gnu++23 -stdlib=libc++ $common @SRC@ @MAIN@ -pthread -o @OUT@"
fi
if [ "$want" = all ] || [ "$want" = libstdcxx ]; then
  run_backend libstdcxx "g++-13 -std=gnu++23 $common @SRC@ @MAIN@ -pthread -o @OUT@"
fi
