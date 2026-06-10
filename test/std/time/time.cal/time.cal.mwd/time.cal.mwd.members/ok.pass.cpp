// AST-transferred from libc++ by tools/transfer.py (slug=time_time_cal_time_cal_mwd_time_cal_mwd_members_ok).
// main -> test_time_time_cal_time_cal_mwd_time_cal_mwd_members_ok; file-scope helpers isolated in anon namespace.
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

// constexpr bool ok() const noexcept;
//  Returns: m_.ok() && wdi_.ok().

#include <chrono>
#include <cassert>
#include <type_traits>
#include <utility>

#include "test_macros.h"

void test_time_time_cal_time_cal_mwd_time_cal_mwd_members_ok()
{
    using month_weekday   = std::chrono::month_weekday;
    using month           = std::chrono::month;
    using weekday         = std::chrono::weekday;
    using weekday_indexed = std::chrono::weekday_indexed;

    constexpr weekday Sunday = std::chrono::Sunday;

    ASSERT_NOEXCEPT(                std::declval<const month_weekday>().ok());
    ASSERT_SAME_TYPE(bool, decltype(std::declval<const month_weekday>().ok()));

    static_assert(!month_weekday{month{}, weekday_indexed{}}.ok(),                   "");
    static_assert( month_weekday{std::chrono::May, weekday_indexed{Sunday, 2}}.ok(), "");

    assert(!(month_weekday(std::chrono::April, weekday_indexed{Sunday, 0}).ok()));
    assert( (month_weekday{std::chrono::March, weekday_indexed{Sunday, 1}}.ok()));

    for (unsigned i = 1; i <= 12; ++i)
        for (unsigned j = 0; j <= 6; ++j)
        {
            month_weekday mwd{month{i}, weekday_indexed{Sunday, j}};
            assert(mwd.ok() == (j >= 1 && j <= 5));
        }

    //  If the month is not ok, all the weekday_indexed are bad
    for (unsigned i = 1; i <= 10; ++i)
        assert(!(month_weekday{month{13}, weekday_indexed{Sunday, i}}.ok()));

    return;
}
