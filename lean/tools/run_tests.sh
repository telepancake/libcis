#!/usr/bin/env bash
# Build + run lean/test/*.cpp against the lean overlay, at -O0 -g AND -Os.
#   lean/tools/run_tests.sh [filter]     # filter: substring of the test name
#   CXX=clang++ lean/tools/run_tests.sh
# Exit 0 iff every selected test compiled and ran clean. Fails loudly;
# nothing is skipped silently.
set -u
cd "$(dirname "$0")/../.."
CXX=${CXX:-g++}
FILTER=${1:-}
LINK="src/support.cpp lean/src/kernels.cpp -nodefaultlibs -lpthread -lm -lc -lgcc_s -lgcc"
INC="-nostdinc++ -Ilean/include -Iinclude"
out=$(mktemp -d)
trap 'rm -rf "$out"' EXIT
fail=0
ran=0
for t in lean/test/*.cpp; do
  name=$(basename "$t" .cpp)
  case "$name" in *"$FILTER"*) ;; *) continue ;; esac
  for opt in "-O0 -g" "-Os"; do
    ran=$((ran+1))
    exe="$out/$name$(echo "$opt" | tr -d ' -')"
    if ! $CXX -std=gnu++20 -fcoroutines -fno-exceptions -fno-rtti $INC $opt -w \
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
