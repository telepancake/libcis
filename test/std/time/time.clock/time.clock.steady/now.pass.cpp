// AST-transferred from libc++ by tools/transfer.py (slug=time_time_clock_time_clock_steady_now).
// main -> test_time_time_clock_time_clock_steady_now; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: no-monotonic-clock

// <chrono>

// steady_clock

// static time_point now();

#include <chrono>
#include <cassert>

#include "test_macros.h"

void test_time_time_clock_time_clock_steady_now()
{
    typedef std::chrono::steady_clock C;
    C::time_point t1 = C::now();
    C::time_point t2 = C::now();
    assert(t2 >= t1);
    // make sure t2 didn't wrap around
    assert(t2 > std::chrono::time_point<C>());

  return;
}
