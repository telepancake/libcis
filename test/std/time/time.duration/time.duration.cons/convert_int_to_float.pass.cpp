// AST-transferred from libc++ by tools/transfer.py (slug=time_time_duration_time_duration_cons_convert_int_to_float).
// main -> test_time_time_duration_time_duration_cons_convert_int_to_float; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <chrono>

// duration

// template <class Rep2, class Period2>
//   duration(const duration<Rep2, Period2>& d);

//  conversions from integral to floating point durations allowed

#include <chrono>
#include <cassert>
#include <ratio>

#include "test_macros.h"

void test_time_time_duration_time_duration_cons_convert_int_to_float()
{
    {
    std::chrono::duration<int> i(3);
    std::chrono::duration<double, std::milli> d = i;
    assert(d.count() == 3000);
    }
#if TEST_STD_VER >= 11
    {
    constexpr std::chrono::duration<int> i(3);
    constexpr std::chrono::duration<double, std::milli> d = i;
    static_assert(d.count() == 3000, "");
    }
#endif

  return;
}
