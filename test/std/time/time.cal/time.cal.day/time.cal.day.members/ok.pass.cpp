// transferred+adapted from libc++ by tools/transfer.py (slug=time_time_cal_time_cal_day_time_cal_day_members_ok_2090d890).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <chrono>
// class day;

// constexpr bool ok() const noexcept;
//  Returns: 1 <= d_ && d_ <= 31

#include <chrono>
#include <cassert>
#include <type_traits>
#include <utility>

#include "test_macros.h"

namespace libcis_ns_time_time_cal_time_cal_day_time_cal_day_members_ok_2090d890 { // libcis
int main(int, char**)
{
    using day = std::chrono::day;
    ASSERT_NOEXCEPT(                std::declval<const day>().ok());
    ASSERT_SAME_TYPE(bool, decltype(std::declval<const day>().ok()));

    static_assert(!day{0}.ok(), "");
    static_assert( day{1}.ok(), "");

    assert(!day{0}.ok());
    for (unsigned i = 1; i <= 31; ++i)
        assert(day{i}.ok());
    for (unsigned i = 32; i <= 255; ++i)
        assert(!day{i}.ok());

  return 0;

    return 0;
}
} // libcis_ns_time_time_cal_time_cal_day_time_cal_day_members_ok_2090d890 (libcis)

