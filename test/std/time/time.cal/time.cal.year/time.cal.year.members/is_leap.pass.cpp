// AST-transferred from libc++ by tools/transfer.py (slug=time_time_cal_time_cal_year_time_cal_year_members_is_leap).
// main -> test_time_time_cal_time_cal_year_time_cal_year_members_is_leap; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <chrono>
// class year;

// constexpr bool is_leap() const noexcept;
//  y_ % 4 == 0 && (y_ % 100 != 0 || y_ % 400 == 0)
//

#include <chrono>
#include <type_traits>
#include <cassert>

#include "test_macros.h"

void test_time_time_cal_time_cal_year_time_cal_year_members_is_leap()
{
    using year = std::chrono::year;

    ASSERT_NOEXCEPT(                year(1).is_leap());
    ASSERT_SAME_TYPE(bool, decltype(year(1).is_leap()));

    static_assert(!year{1}.is_leap(), "");
    static_assert(!year{2}.is_leap(), "");
    static_assert(!year{3}.is_leap(), "");
    static_assert( year{4}.is_leap(), "");

    assert( year{-2000}.is_leap());
    assert( year{ -400}.is_leap());
    assert(!year{ -300}.is_leap());
    assert(!year{ -200}.is_leap());

    assert(!year{  200}.is_leap());
    assert(!year{  300}.is_leap());
    assert( year{  400}.is_leap());
    assert(!year{ 1997}.is_leap());
    assert(!year{ 1998}.is_leap());
    assert(!year{ 1999}.is_leap());
    assert( year{ 2000}.is_leap());
    assert(!year{ 2001}.is_leap());
    assert(!year{ 2002}.is_leap());
    assert(!year{ 2003}.is_leap());
    assert( year{ 2004}.is_leap());
    assert(!year{ 2100}.is_leap());

  return;
}
