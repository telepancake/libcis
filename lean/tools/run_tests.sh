#!/usr/bin/env bash
# Build + run lean/test/*.cpp against the lean overlay, at -O0 -g AND -Os.
#   lean/tools/run_tests.sh [filter]     # filter: substring of the test name
#   CXX=clang++ lean/tools/run_tests.sh
#   CXXARCH=-m32 lean/tools/run_tests.sh # run the whole suite as 32-bit (i386)
# Exit 0 iff every selected test compiled and ran clean. Fails loudly;
# nothing is skipped silently.
#
# CXXARCH: an architecture flag threaded through EVERY compile AND link (empty
# by default, so the 64-bit build is byte-identical to before). `CXXARCH=-m32`
# builds and runs the entire suite for 32-bit i386. The flag is validated up
# front: if the toolchain cannot produce a runnable binary for it, we abort
# loudly rather than silently skipping the arch.
set -u
cd "$(dirname "$0")/../.."
CXX=${CXX:-g++}
CXXARCH=${CXXARCH:-}
FILTER=${1:-}
LINK="src/support.cpp lean/src/kernels.cpp -nodefaultlibs -lpthread -lm -lc -lgcc_s -lgcc"
INC="-nostdinc++ -Ilean/include -Iinclude"
out=$(mktemp -d)
trap 'rm -rf "$out"' EXIT

# Fail loudly if CXXARCH is set but the toolchain can't compile+run for it.
if [ -n "$CXXARCH" ]; then
  probe="$out/arch_probe"
  printf 'int main(){return 0;}\n' > "$probe.cpp"
  if ! $CXX $CXXARCH "$probe.cpp" -o "$probe" 2>"$probe.err"; then
    echo "NOT CLEAN (CXXARCH=$CXXARCH cannot compile — no silent skip)"
    sed -n '1,20p' "$probe.err"
    exit 1
  fi
  if ! "$probe"; then
    echo "NOT CLEAN (CXXARCH=$CXXARCH compiled but the binary did not run — no silent skip)"
    exit 1
  fi
  echo "arch: building the suite with CXXARCH=$CXXARCH"
fi

fail=0
ran=0
for t in lean/test/*.cpp; do
  name=$(basename "$t" .cpp)
  case "$name" in *"$FILTER"*) ;; *) continue ;; esac
  for opt in "-O0 -g" "-Os"; do
    ran=$((ran+1))
    exe="$out/$name$(echo "$opt" | tr -d ' -')"
    if ! $CXX -std=gnu++20 -fcoroutines -fno-exceptions -fno-rtti $CXXARCH $INC $opt -w \
         "$t" $LINK -o "$exe" 2>"$exe.err"; then
      echo "FAIL(compile $opt) $t"
      sed -n '1,20p' "$exe.err"
      fail=1
      continue
    fi
    if ! "$exe"; then
      echo "FAIL(run $opt) $t"
      fail=1
      continue
    fi
    echo "ok ($opt) $t"
  done
done
if [ "$ran" = 0 ]; then
  echo "NOT CLEAN (no tests matched filter '$FILTER')"
  exit 1
fi
[ "$fail" = 0 ] && echo CLEAN || echo "NOT CLEAN"
exit $fail
