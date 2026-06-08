#!/usr/bin/env bash
# Build and run the libcis test suite.
#
# Generates build/test_main.cpp from all test/*.cpp, compiles everything against
# the libcis headers with the target flags, and runs it. A silent exit 0 means
# every test passed; a crash means a test failed (run under gdb/valgrind for a
# backtrace).
set -euo pipefail

cd "$(dirname "$0")"

CXX="${CXX:-g++}"
STD="${STD:-gnu++20}"
FLAGS="-std=$STD -fno-exceptions -fno-rtti -nostdinc++ -Iinclude -g -O0 -Wall -Wextra"

mkdir -p build
tests=(test/*.cpp)

tools/gen_main.sh build/test_main.cpp "${tests[@]}"

$CXX $FLAGS "${tests[@]}" build/test_main.cpp -o build/tests
exec build/tests "$@"
