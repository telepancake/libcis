// transferred+adapted from libc++ by tools/transfer.py (slug=time_time_cal_time_cal_wdlast_time_cal_wdlast_members_weekday_815766ee).
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

//  constexpr chrono::weekday weekday() const noexcept;
//  Returns: wd_

#include <chrono>
#include <cassert>
#include <type_traits>
#include <utility>

#include "test_macros.h"

namespace libcis_ns_time_time_cal_time_cal_wdlast_time_cal_wdlast_members_weekday_815766ee { // libcis
int main(int, char**)
{
    using weekday      = std::chrono::weekday;
    using weekday_last = std::chrono::weekday_last;

    ASSERT_NOEXCEPT(                   std::declval<const weekday_last>().weekday());
    ASSERT_SAME_TYPE(weekday, decltype(std::declval<const weekday_last>().weekday()));

    for (unsigned i = 0; i <= 255; ++i)
        assert(weekday_last{weekday{i}}.weekday() == weekday{i});

  return 0;

    return 0;
}
} // libcis_ns_time_time_cal_time_cal_wdlast_time_cal_wdlast_members_weekday_815766ee (libcis)

