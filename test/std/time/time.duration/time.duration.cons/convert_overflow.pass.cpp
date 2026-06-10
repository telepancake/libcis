// AST-transferred from libc++ by tools/transfer.py (slug=time_time_duration_time_duration_cons_convert_overflow).
// main -> test_time_time_duration_time_duration_cons_convert_overflow; file-scope helpers isolated in anon namespace.
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

// overflow should SFINAE instead of error out, LWG 2094

#include <chrono>
#include <cassert>
#include <ratio>

#include "test_macros.h"

namespace libcis_ns_time_time_duration_time_duration_cons_convert_overflow { // libcis: isolate file-scope helpers
bool called = false;

void f(std::chrono::milliseconds);
void f(std::chrono::seconds)
{
    called = true;
}
} using namespace libcis_ns_time_time_duration_time_duration_cons_convert_overflow; // libcis


void test_time_time_duration_time_duration_cons_convert_overflow()
{
    {
    std::chrono::duration<int, std::exa> r(1);
    f(r);
    assert(called);
    }

  return;
}
