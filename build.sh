#!/usr/bin/env bash
# Build and run the libcis test suite.
#
# libcis is self-contained: it links WITHOUT libstdc++ (-nodefaultlibs), using
# only libc + the compiler runtime, plus its own C++ runtime support in
# src/support.cpp (operator new/delete, the __cxa ABI glue, std::exception and
# the new_handler/terminate machinery). A silent exit 0 means every test passed;
# a crash means a test failed (run under gdb/valgrind for a backtrace).
set -euo pipefail

cd "$(dirname "$0")"

CXX="${CXX:-g++}"
STD="${STD:-gnu++20}"
FLAGS="-std=$STD -fno-exceptions -fno-rtti -nostdinc++ -Iinclude -g -O0 -Wall -Wextra -pthread"
# No libstdc++: drop the default libs, add back libc + compiler runtime + pthread/m.
LINK="-nodefaultlibs -lpthread -lm -lc -lgcc_s -lgcc"

mkdir -p build
tests=(test/*.cpp)

tools/gen_main.sh build/test_main.cpp "${tests[@]}"

$CXX $FLAGS "${tests[@]}" src/support.cpp build/test_main.cpp -o build/tests $LINK
exec build/tests "$@"
