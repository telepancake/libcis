// AST-transferred from libc++ by tools/transfer.py (slug=time_time_duration_types).
// main -> test_time_time_duration_types; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <chrono>

// duration

// Test nested types

// typedef Rep rep;
// typedef Period period;

#include <chrono>
#include <ratio>
#include <type_traits>

#include "test_macros.h"

void test_time_time_duration_types()
{
    typedef std::chrono::duration<long, std::ratio<3, 2> > D;
    static_assert((std::is_same<D::rep, long>::value), "");
    static_assert((std::is_same<D::period, std::ratio<3, 2> >::value), "");

  return;
}
