// AST-transferred from libc++ by tools/transfer.py (slug=time_time_cal_time_cal_ymwd_time_cal_ymwd_members_year).
// main -> test_time_time_cal_time_cal_ymwd_time_cal_ymwd_members_year; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <chrono>
// class year_month_weekday;

// constexpr chrono::year year() const noexcept;
//  Returns: d_

#include <chrono>
#include <cassert>
#include <type_traits>
#include <utility>

#include "test_macros.h"

void test_time_time_cal_time_cal_ymwd_time_cal_ymwd_members_year()
{
    using year               = std::chrono::year;
    using month              = std::chrono::month;
    using weekday_indexed    = std::chrono::weekday_indexed;
    using year_month_weekday = std::chrono::year_month_weekday;

    ASSERT_NOEXCEPT(                std::declval<const year_month_weekday>().year());
    ASSERT_SAME_TYPE(year, decltype(std::declval<const year_month_weekday>().year()));

    static_assert( year_month_weekday{}.year() == year{}, "");

    for (int i = 1; i <= 50; ++i)
    {
        year_month_weekday ym(year{i}, month{1}, weekday_indexed{});
        assert( static_cast<int>(ym.year()) == i);
    }

  return;
}
