// transferred+adapted from libc++ by tools/transfer.py (slug=time_time_duration_time_duration_arithmetic_op_63d8b8cf).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <chrono>

// duration

// duration& operator-=(const duration& d);

#include <chrono>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_time_time_duration_time_duration_arithmetic_op_63d8b8cf { // libcis
#if TEST_STD_VER > 14
constexpr bool test_constexpr()
{
    std::chrono::seconds s(3);
    s -= std::chrono::seconds(2);
    if (s.count() != 1) return false;
    s -= std::chrono::minutes(2);
    return s.count() == -119;
}
#endif

int main(int, char**)
{
    {
    std::chrono::seconds s(3);
    s -= std::chrono::seconds(2);
    assert(s.count() == 1);
    s -= std::chrono::minutes(2);
    assert(s.count() == -119);
    }

#if TEST_STD_VER > 14
    static_assert(test_constexpr(), "");
#endif

  return 0;

    return 0;
}
} // libcis_ns_time_time_duration_time_duration_arithmetic_op_63d8b8cf (libcis)

