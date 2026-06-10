// AST-transferred from libc++ by tools/transfer.py (slug=time_time_cal_time_cal_mdlast_month).
// main -> test_time_time_cal_time_cal_mdlast_month; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <chrono>
// class month_day_last;

// constexpr chrono::month month() const noexcept;
//  Returns: m_

#include <chrono>
#include <cassert>
#include <type_traits>
#include <utility>

#include "test_macros.h"

void test_time_time_cal_time_cal_mdlast_month()
{
    using month     = std::chrono::month;
    using month_day_last = std::chrono::month_day_last;

    ASSERT_NOEXCEPT(                 std::declval<const month_day_last>().month());
    ASSERT_SAME_TYPE(month, decltype(std::declval<const month_day_last>().month()));

    static_assert( month_day_last{month{}}.month() == month{}, "");

    for (unsigned i = 1; i <= 50; ++i)
    {
        month_day_last mdl(month{i});
        assert( static_cast<unsigned>(mdl.month()) == i);
    }

  return;
}
