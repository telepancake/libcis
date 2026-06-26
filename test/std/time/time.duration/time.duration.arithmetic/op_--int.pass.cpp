// transferred+adapted from libc++ by tools/transfer.py (slug=time_time_duration_time_duration_arithmetic_op_int_bd9b79ac).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <chrono>

// duration

// constexpr duration operator--(int);   // constexpr in C++17

#include <chrono>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_time_time_duration_time_duration_arithmetic_op_int_bd9b79ac { // libcis
#if TEST_STD_VER > 14
constexpr bool test_constexpr()
{
    std::chrono::hours h1(3);
    std::chrono::hours h2 = h1--;
    return h1.count() == 2 && h2.count() == 3;
}
#endif


int main(int, char**)
{
    {
    std::chrono::hours h1(3);
    std::chrono::hours h2 = h1--;
    assert(h1.count() == 2);
    assert(h2.count() == 3);
    }

#if TEST_STD_VER > 14
    static_assert(test_constexpr(), "");
#endif

  return 0;

    return 0;
}
} // libcis_ns_time_time_duration_time_duration_arithmetic_op_int_bd9b79ac (libcis)

