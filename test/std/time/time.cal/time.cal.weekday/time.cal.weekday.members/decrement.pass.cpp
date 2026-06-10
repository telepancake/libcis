// AST-transferred from libc++ by tools/transfer.py (slug=time_time_cal_time_cal_weekday_time_cal_weekday_members_decrement).
// main -> test_time_time_cal_time_cal_weekday_time_cal_weekday_members_decrement; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <chrono>
// class weekday;

//  constexpr weekday& operator--() noexcept;
//  constexpr weekday operator--(int) noexcept;

#include <chrono>
#include <cassert>
#include <type_traits>
#include <utility>

#include "test_macros.h"
#include "../../euclidian.h"

namespace libcis_ns_time_time_cal_time_cal_weekday_time_cal_weekday_members_decrement { // libcis: isolate file-scope helpers
using weekday = std::chrono::weekday;

constexpr bool test() {
  for (unsigned i = 0; i <= 6; ++i) {
    weekday wd(i);
    assert(((--wd).c_encoding() == euclidian_subtraction<unsigned, 0, 6>(i, 1)));
    assert(((wd--).c_encoding() == euclidian_subtraction<unsigned, 0, 6>(i, 1)));
    assert(((wd).c_encoding() == euclidian_subtraction<unsigned, 0, 6>(i, 2)));
  }

  return true;
}
} using namespace libcis_ns_time_time_cal_time_cal_weekday_time_cal_weekday_members_decrement; // libcis


void test_time_time_cal_time_cal_weekday_time_cal_weekday_members_decrement() {
  ASSERT_NOEXCEPT(--(std::declval<weekday&>()));
  ASSERT_NOEXCEPT((std::declval<weekday&>())--);

  ASSERT_SAME_TYPE(weekday, decltype(std::declval<weekday&>()--));
  ASSERT_SAME_TYPE(weekday&, decltype(--std::declval<weekday&>()));

  test();
  static_assert(test());

  return;
}
