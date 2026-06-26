// transferred+adapted from libc++ by tools/transfer.py (slug=time_time_cal_time_cal_ymd_time_cal_ymd_members_year_a9a9527c).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <chrono>
// class year_month_day;

// constexpr chrono::day day() const noexcept;
//  Returns: d_

#include <chrono>
#include <cassert>
#include <type_traits>
#include <utility>

#include "test_macros.h"

namespace libcis_ns_time_time_cal_time_cal_ymd_time_cal_ymd_members_year_a9a9527c { // libcis
int main(int, char**)
{
    using year           = std::chrono::year;
    using month          = std::chrono::month;
    using day            = std::chrono::day;
    using year_month_day = std::chrono::year_month_day;

    ASSERT_NOEXCEPT(                std::declval<const year_month_day>().year());
    ASSERT_SAME_TYPE(year, decltype(std::declval<const year_month_day>().year()));

    static_assert( year_month_day{}.year() == year{}, "");

    for (int i = 1; i <= 50; ++i)
    {
        year_month_day ym(year{i}, month{}, day{});
        assert( static_cast<int>(ym.year()) == i);
    }

  return 0;

    return 0;
}
} // libcis_ns_time_time_cal_time_cal_ymd_time_cal_ymd_members_year_a9a9527c (libcis)

