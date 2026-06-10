// AST-transferred from libc++ by tools/transfer.py (slug=time_time_cal_time_cal_day_types).
// main -> test_time_time_cal_time_cal_day_types; file-scope helpers isolated in anon namespace.
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

#include <chrono>
#include <type_traits>
#include <cassert>

#include "test_macros.h"

void test_time_time_cal_time_cal_day_types()
{
    using day = std::chrono::day;

    static_assert(std::is_trivially_copyable_v<day>, "");
    static_assert(std::is_standard_layout_v<day>, "");

  return;
}
