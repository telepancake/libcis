// transferred+adapted from libc++ by tools/transfer.py (slug=time_time_duration_time_duration_arithmetic_op_mod_duration_f34b9557).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <chrono>

// duration

// duration& operator%=(const duration& rhs)

#include <chrono>
#include <cassert>

#include "test_macros.h"
#include "../../rep.h"

namespace libcis_ns_time_time_duration_time_duration_arithmetic_op_mod_duration_f34b9557 { // libcis
#if TEST_STD_VER > 14
constexpr bool test_constexpr()
{
    std::chrono::microseconds us1(11);
    std::chrono::microseconds us2(3);
    us1 %= us2;
    return us1.count() == 2;
}
#endif

int main(int, char**)
{
    {
    std::chrono::microseconds us1(11);
    std::chrono::microseconds us2(3);
    us1 %= us2;
    assert(us1.count() == 2);
    us1 %= std::chrono::milliseconds(3);
    assert(us1.count() == 2);
    }

#if TEST_STD_VER > 14
    static_assert(test_constexpr(), "");
#endif

#if TEST_STD_VER >= 11
    { // This is PR#41130
    std::chrono::nanoseconds d(5);
    NotARep n;
    d %= n;
    assert(d.count() == 5);
    }
#endif

  return 0;

    return 0;
}
} // libcis_ns_time_time_duration_time_duration_arithmetic_op_mod_duration_f34b9557 (libcis)

