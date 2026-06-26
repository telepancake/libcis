// transferred+adapted from libc++ by tools/transfer.py (slug=time_time_traits_time_traits_duration_values_zero_0fbf73cb).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <chrono>

// duration_values::zero  // noexcept after C++17

#include <chrono>
#include <cassert>

#include "test_macros.h"
#include "../../rep.h"

namespace libcis_ns_time_time_traits_time_traits_duration_values_zero_0fbf73cb { // libcis
int main(int, char**)
{
    assert(std::chrono::duration_values<int>::zero() == 0);
    assert(std::chrono::duration_values<Rep>::zero() == 0);
#if TEST_STD_VER >= 11
    static_assert(std::chrono::duration_values<int>::zero() == 0, "");
    static_assert(std::chrono::duration_values<Rep>::zero() == 0, "");
#endif

    LIBCPP_ASSERT_NOEXCEPT(std::chrono::duration_values<int>::zero());
    LIBCPP_ASSERT_NOEXCEPT(std::chrono::duration_values<Rep>::zero());
#if TEST_STD_VER > 17
    ASSERT_NOEXCEPT(std::chrono::duration_values<int>::zero());
    ASSERT_NOEXCEPT(std::chrono::duration_values<Rep>::zero());
#endif

  return 0;

    return 0;
}
} // libcis_ns_time_time_traits_time_traits_duration_values_zero_0fbf73cb (libcis)

