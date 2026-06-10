// AST-transferred from libc++ by tools/transfer.py (slug=time_time_duration_default_ratio).
// main -> test_time_time_duration_default_ratio; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <chrono>

// duration

// Test default template arg:

// template <class Rep, class Period = ratio<1>>
// class duration;

#include <chrono>
#include <ratio>
#include <type_traits>

#include "test_macros.h"

void test_time_time_duration_default_ratio()
{
    static_assert((std::is_same<std::chrono::duration<int, std::ratio<1> >,
                   std::chrono::duration<int> >::value), "");

  return;
}
