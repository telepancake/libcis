// AST-transferred from libc++ by tools/transfer.py (slug=time_time_cal_time_cal_day_time_cal_day_nonmembers_plus).
// main -> test_time_time_cal_time_cal_day_time_cal_day_nonmembers_plus; file-scope helpers isolated in anon namespace.
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

// constexpr day operator+(const day& x, const days& y) noexcept;
//   Returns: day(unsigned{x} + y.count()).
//
// constexpr day operator+(const days& x, const day& y) noexcept;
//   Returns: y + x.

#include <chrono>
#include <cassert>
#include <type_traits>
#include <utility>

#include "test_macros.h"

namespace libcis_ns_time_time_cal_time_cal_day_time_cal_day_nonmembers_plus { // libcis: isolate file-scope helpers
using day  = std::chrono::day;
using days = std::chrono::days;

constexpr bool test() {
  day dy{12};
  for (unsigned i = 0; i <= 10; ++i) {
    day d1 = dy + days{i};
    day d2 = days{i} + dy;
    assert(d1 == d2);
    assert(static_cast<unsigned>(d1) == i + 12);
    assert(static_cast<unsigned>(d2) == i + 12);
  }

  return true;
}
} using namespace libcis_ns_time_time_cal_time_cal_day_time_cal_day_nonmembers_plus; // libcis


void test_time_time_cal_time_cal_day_time_cal_day_nonmembers_plus() {
  ASSERT_NOEXCEPT(std::declval<day>() + std::declval<days>());
  ASSERT_NOEXCEPT(std::declval<days>() + std::declval<day>());

  ASSERT_SAME_TYPE(day, decltype(std::declval<day>() + std::declval<days>()));
  ASSERT_SAME_TYPE(day, decltype(std::declval<days>() + std::declval<day>()));

  test();
  static_assert(test());

  return;
}
