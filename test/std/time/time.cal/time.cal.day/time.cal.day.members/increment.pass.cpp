// transferred+adapted from libc++ by tools/transfer.py (slug=time_time_cal_time_cal_day_time_cal_day_members_increment_11472621).
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

//  constexpr day& operator++() noexcept;
//  constexpr day operator++(int) noexcept;

#include <chrono>
#include <cassert>
#include <type_traits>
#include <utility>

#include "test_macros.h"

namespace libcis_ns_time_time_cal_time_cal_day_time_cal_day_members_increment_11472621 { // libcis
using day = std::chrono::day;

constexpr bool test() {
  for (unsigned i = 10; i <= 20; ++i) {
    day d(i);
    assert(static_cast<unsigned>(++d) == i + 1);
    assert(static_cast<unsigned>(d++) == i + 1);
    assert(static_cast<unsigned>(d) == i + 2);
  }

  return true;
}

int main(int, char**) {
  ASSERT_NOEXCEPT(++(std::declval<day&>()));
  ASSERT_NOEXCEPT((std::declval<day&>())++);

  ASSERT_SAME_TYPE(day, decltype(std::declval<day&>()++));
  ASSERT_SAME_TYPE(day&, decltype(++std::declval<day&>()));

  test();
  static_assert(test());

  return 0;

    return 0;
}
} // libcis_ns_time_time_cal_time_cal_day_time_cal_day_members_increment_11472621 (libcis)

