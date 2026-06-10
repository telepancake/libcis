// AST-transferred from libc++ by tools/transfer.py (slug=time_time_point_time_point_cons_default).
// main -> test_time_time_point_time_point_cons_default; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <chrono>

// time_point

// time_point();

#include <chrono>
#include <cassert>
#include <ratio>

#include "test_macros.h"
#include "../../rep.h"

void test_time_time_point_time_point_cons_default()
{
    typedef std::chrono::system_clock Clock;
    typedef std::chrono::duration<Rep, std::milli> Duration;
    {
    std::chrono::time_point<Clock, Duration> t;
    assert(t.time_since_epoch() == Duration::zero());
    }
#if TEST_STD_VER > 11
    {
    constexpr std::chrono::time_point<Clock, Duration> t;
    static_assert(t.time_since_epoch() == Duration::zero(), "");
    }
#endif

  return;
}
