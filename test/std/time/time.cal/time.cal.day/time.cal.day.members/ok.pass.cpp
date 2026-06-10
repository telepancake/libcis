// AST-transferred from libc++ by tools/transfer.py (slug=time_time_cal_time_cal_day_time_cal_day_members_ok).
// main -> test_time_time_cal_time_cal_day_time_cal_day_members_ok; file-scope helpers isolated in anon namespace.
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

void test_time_time_cal_time_cal_day_time_cal_day_members_ok()
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

  return;
}
