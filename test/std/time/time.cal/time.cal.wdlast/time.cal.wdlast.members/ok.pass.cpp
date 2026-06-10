// AST-transferred from libc++ by tools/transfer.py (slug=time_time_cal_time_cal_wdlast_time_cal_wdlast_members_ok).
// main -> test_time_time_cal_time_cal_wdlast_time_cal_wdlast_members_ok; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <chrono>
// class weekday_last;

//  constexpr bool ok() const noexcept;
//  Returns: wd_.ok()

#include <chrono>
#include <cassert>
#include <type_traits>
#include <utility>

#include "test_macros.h"

void test_time_time_cal_time_cal_wdlast_time_cal_wdlast_members_ok()
{
    using weekday      = std::chrono::weekday;
    using weekday_last = std::chrono::weekday_last;

    ASSERT_NOEXCEPT(                std::declval<const weekday_last>().ok());
    ASSERT_SAME_TYPE(bool, decltype(std::declval<const weekday_last>().ok()));

    static_assert( weekday_last{weekday{0}}.ok(), "");
    static_assert( weekday_last{weekday{1}}.ok(), "");
    static_assert(!weekday_last{weekday{8}}.ok(), "");

    for (unsigned i = 0; i <= 255; ++i)
        assert(weekday_last{weekday{i}}.ok() == weekday{i}.ok());

  return;
}
