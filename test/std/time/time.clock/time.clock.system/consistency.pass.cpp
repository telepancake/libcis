// AST-transferred from libc++ by tools/transfer.py (slug=time_time_clock_time_clock_system_consistency).
// main -> test_time_time_clock_time_clock_system_consistency; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Due to C++17 inline variables ASAN flags this test as containing an ODR
// violation because Clock::is_steady is defined in both the dylib and this TU.
// UNSUPPORTED: asan

// <chrono>

// system_clock

// check clock invariants

#include <chrono>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_time_time_clock_time_clock_system_consistency { // libcis: isolate file-scope helpers
template <class T>
void test(const T &) {}
} using namespace libcis_ns_time_time_clock_time_clock_system_consistency; // libcis


void test_time_time_clock_time_clock_system_consistency()
{
    typedef std::chrono::system_clock C;
    static_assert((std::is_same<C::rep, C::duration::rep>::value), "");
    static_assert((std::is_same<C::period, C::duration::period>::value), "");
    static_assert((std::is_same<C::duration, C::time_point::duration>::value), "");
    static_assert((std::is_same<C::time_point::clock, C>::value), "");
    static_assert((C::is_steady || !C::is_steady), "");
    test(std::chrono::system_clock::is_steady);

  return;
}
