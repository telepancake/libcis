// AST-transferred from libc++ by tools/transfer.py (slug=time_time_duration_time_duration_arithmetic_op_times).
// main -> test_time_time_duration_time_duration_arithmetic_op_times; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <chrono>

// duration

// duration& operator*=(const rep& rhs);

#include <chrono>
#include <cassert>

#include "test_macros.h"
#include "../../rep.h"

#if TEST_STD_VER > 14
namespace libcis_ns_time_time_duration_time_duration_arithmetic_op_times { // libcis: isolate file-scope helpers
constexpr bool test_constexpr()
{
    std::chrono::seconds s(3);
    s *= 5;
    return s.count() == 15;
}
} using namespace libcis_ns_time_time_duration_time_duration_arithmetic_op_times; // libcis

#endif

void test_time_time_duration_time_duration_arithmetic_op_times()
{
    {
    std::chrono::nanoseconds ns(3);
    ns *= 5;
    assert(ns.count() == 15);
    }

#if TEST_STD_VER > 14
    static_assert(test_constexpr(), "");
#endif

#if TEST_STD_VER >= 11
    { // This is PR#41130
    std::chrono::nanoseconds d(5);
    NotARep n;
    d *= n;
    assert(d.count() == 5);
    }
#endif

  return;
}
