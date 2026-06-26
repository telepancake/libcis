// transferred+adapted from libc++ by tools/transfer.py (slug=time_time_cal_time_cal_mwd_time_cal_mwd_members_month_be4557e2).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <chrono>
// class month_weekday;

// constexpr chrono::month month() const noexcept;
//  Returns: wd_

#include <chrono>
#include <cassert>
#include <type_traits>
#include <utility>

#include "test_macros.h"

namespace libcis_ns_time_time_cal_time_cal_mwd_time_cal_mwd_members_month_be4557e2 { // libcis
int main(int, char**)
{
    using month_weekday   = std::chrono::month_weekday;
    using month           = std::chrono::month;
    using weekday         = std::chrono::weekday;
    using weekday_indexed = std::chrono::weekday_indexed;

    constexpr weekday Sunday = std::chrono::Sunday;

    ASSERT_NOEXCEPT(                 std::declval<const month_weekday>().month());
    ASSERT_SAME_TYPE(month, decltype(std::declval<const month_weekday>().month()));

    for (unsigned i = 1; i <= 50; ++i)
    {
        month_weekday md(month{i}, weekday_indexed{Sunday, 1});
        assert( static_cast<unsigned>(md.month()) == i);
    }

  return 0;

    return 0;
}
} // libcis_ns_time_time_cal_time_cal_mwd_time_cal_mwd_members_month_be4557e2 (libcis)

