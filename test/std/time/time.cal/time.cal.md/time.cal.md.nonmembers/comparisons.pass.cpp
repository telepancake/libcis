// AST-transferred from libc++ by tools/transfer.py (slug=time_time_cal_time_cal_md_time_cal_md_nonmembers_comparisons).
// main -> test_time_time_cal_time_cal_md_time_cal_md_nonmembers_comparisons; file-scope helpers isolated in anon namespace.
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

// constexpr bool operator==(const month_day& x, const month_day& y) noexcept;
// constexpr strong_comparison operator<=>(const month_day& x, const month_day& y) noexcept;

#include <chrono>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "test_comparisons.h"

namespace libcis_ns_time_time_cal_time_cal_md_time_cal_md_nonmembers_comparisons { // libcis: isolate file-scope helpers
constexpr bool test() {
  using day       = std::chrono::day;
  using month     = std::chrono::month;
  using month_day = std::chrono::month_day;

  assert(testOrder(
      month_day{std::chrono::January, day{1}}, month_day{std::chrono::January, day{1}}, std::strong_ordering::equal));

  assert(testOrder(
      month_day{std::chrono::January, day{1}}, month_day{std::chrono::January, day{2}}, std::strong_ordering::less));

  assert(testOrder(
      month_day{std::chrono::January, day{1}}, month_day{std::chrono::February, day{1}}, std::strong_ordering::less));

  //  same day, different months
  for (unsigned i = 1; i < 12; ++i)
    for (unsigned j = 1; j < 12; ++j)
      assert((testOrder(
          month_day{month{i}, day{1}},
          month_day{month{j}, day{1}},
          i == j  ? std::strong_ordering::equal
          : i < j ? std::strong_ordering::less
                  : std::strong_ordering::greater)));

  //  same month, different days
  for (unsigned i = 1; i < 31; ++i)
    for (unsigned j = 1; j < 31; ++j)
      assert((testOrder(
          month_day{month{2}, day{i}},
          month_day{month{2}, day{j}},
          i == j  ? std::strong_ordering::equal
          : i < j ? std::strong_ordering::less
                  : std::strong_ordering::greater)));

  return true;
}
} using namespace libcis_ns_time_time_cal_time_cal_md_time_cal_md_nonmembers_comparisons; // libcis


void test_time_time_cal_time_cal_md_time_cal_md_nonmembers_comparisons() {
  using month_day = std::chrono::month_day;
  AssertOrderAreNoexcept<month_day>();
  AssertOrderReturn<std::strong_ordering, month_day>();

  test();
  static_assert(test());

  return;
}
