// transferred+adapted from libc++ by tools/transfer.py (slug=time_time_cal_time_cal_month_types_ade003b4).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <chrono>
// class month;

#include <chrono>
#include <type_traits>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_time_time_cal_time_cal_month_types_ade003b4 { // libcis
int main(int, char**)
{
    using month = std::chrono::month;

    static_assert(std::is_trivially_copyable_v<month>, "");
    static_assert(std::is_standard_layout_v<month>, "");

  return 0;

    return 0;
}
} // libcis_ns_time_time_cal_time_cal_month_types_ade003b4 (libcis)

