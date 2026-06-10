// AST-transferred from libc++ by tools/transfer.py (slug=time_time_cal_time_cal_md_time_cal_md_members_day).
// main -> test_time_time_cal_time_cal_md_time_cal_md_members_day; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <chrono>
// class month_day;

// constexpr chrono::day day() const noexcept;
//  Returns: d_

#include <chrono>
#include <cassert>
#include <type_traits>
#include <utility>

#include "test_macros.h"

void test_time_time_cal_time_cal_md_time_cal_md_members_day()
{
    using day       = std::chrono::day;
    using month_day = std::chrono::month_day;

    ASSERT_NOEXCEPT(               std::declval<const month_day>().day());
    ASSERT_SAME_TYPE(day, decltype(std::declval<const month_day>().day()));

    static_assert( month_day{}.day() == day{}, "");

    for (unsigned i = 1; i <= 50; ++i)
    {
        month_day md(std::chrono::March, day{i});
        assert( static_cast<unsigned>(md.day()) == i);
    }

  return;
}
