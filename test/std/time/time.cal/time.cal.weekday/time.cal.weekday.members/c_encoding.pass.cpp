// transferred+adapted from libc++ by tools/transfer.py (slug=time_time_cal_time_cal_weekday_time_cal_weekday_members_c_encoding_fc86f20b).
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

//  constexpr unsigned c_encoding() const noexcept;

#include <chrono>
#include <cassert>
#include <type_traits>
#include <utility>

#include "test_macros.h"

namespace libcis_ns_time_time_cal_time_cal_weekday_time_cal_weekday_members_c_encoding_fc86f20b { // libcis
using weekday = std::chrono::weekday;

constexpr bool test() {
  for (unsigned i = 0; i <= 10; ++i) {
    weekday wd(i);
    assert(wd.c_encoding() == (i == 7 ? 0 : i));
  }

  return true;
}

int main(int, char**) {
  ASSERT_NOEXCEPT(std::declval<weekday&>().c_encoding());
  ASSERT_SAME_TYPE(unsigned, decltype(std::declval<weekday&>().c_encoding()));

  test();
  static_assert(test());

  return 0;

    return 0;
}
} // libcis_ns_time_time_cal_time_cal_weekday_time_cal_weekday_members_c_encoding_fc86f20b (libcis)

