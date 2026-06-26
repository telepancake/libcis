// transferred+adapted from libc++ by tools/transfer.py (slug=time_time_duration_time_duration_arithmetic_op_ed25a6e7).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <chrono>

// duration

// constexpr duration& operator++();  // constexpr in C++17

#include <chrono>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_time_time_duration_time_duration_arithmetic_op_ed25a6e7 { // libcis
#if TEST_STD_VER > 14
constexpr bool test_constexpr()
{
    std::chrono::hours h(3);
    return (++h).count() == 4;
}
#endif

int main(int, char**)
{
    {
    std::chrono::hours h(3);
    std::chrono::hours& href = ++h;
    assert(&href == &h);
    assert(h.count() == 4);
    }

#if TEST_STD_VER > 14
    static_assert(test_constexpr(), "");
#endif

  return 0;

    return 0;
}
} // libcis_ns_time_time_duration_time_duration_arithmetic_op_ed25a6e7 (libcis)

