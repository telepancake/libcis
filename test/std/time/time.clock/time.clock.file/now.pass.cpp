// AST-transferred from libc++ by tools/transfer.py (slug=time_time_clock_time_clock_file_now).
// main -> test_time_time_clock_time_clock_file_now; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// <chrono>

// file_clock

// static time_point now() noexcept;

#include <chrono>
#include <cassert>

#include "test_macros.h"

void test_time_time_clock_time_clock_file_now()
{
    typedef std::chrono::file_clock C;
    ASSERT_NOEXCEPT(C::now());

    C::time_point t1 = C::now();
    assert(t1.time_since_epoch().count() != 0);
    assert(C::time_point::min() < t1);
    assert(C::time_point::max() > t1);

  return;
}
