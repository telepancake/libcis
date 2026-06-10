// AST-transferred from libc++ by tools/transfer.py (slug=time_time_duration_time_duration_cast_duration_cast).
// main -> test_time_time_duration_time_duration_cast_duration_cast; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <chrono>

// duration

// template <class ToDuration, class Rep, class Period>
//   constexpr
//   ToDuration
//   duration_cast(const duration<Rep, Period>& d);

#include <chrono>
#include <cassert>
#include <ratio>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_time_time_duration_time_duration_cast_duration_cast { // libcis: isolate file-scope helpers
template <class ToDuration, class FromDuration>
void
test(const FromDuration& f, const ToDuration& d)
{
    {
    typedef decltype(std::chrono::duration_cast<ToDuration>(f)) R;
    static_assert((std::is_same<R, ToDuration>::value), "");
    assert(std::chrono::duration_cast<ToDuration>(f) == d);
    }
}
} using namespace libcis_ns_time_time_duration_time_duration_cast_duration_cast; // libcis


void test_time_time_duration_time_duration_cast_duration_cast()
{
    test(std::chrono::milliseconds(7265000), std::chrono::hours(2));
    test(std::chrono::milliseconds(7265000), std::chrono::minutes(121));
    test(std::chrono::milliseconds(7265000), std::chrono::seconds(7265));
    test(std::chrono::milliseconds(7265000), std::chrono::milliseconds(7265000));
    test(std::chrono::milliseconds(7265000), std::chrono::microseconds(7265000000LL));
    test(std::chrono::milliseconds(7265000), std::chrono::nanoseconds(7265000000000LL));
    test(std::chrono::milliseconds(7265000),
         std::chrono::duration<double, std::ratio<3600> >(7265./3600));
    test(std::chrono::duration<int, std::ratio<2, 3> >(9),
         std::chrono::duration<int, std::ratio<3, 5> >(10));
#if TEST_STD_VER >= 11
    {
    constexpr std::chrono::hours h = std::chrono::duration_cast<std::chrono::hours>(std::chrono::milliseconds(7265000));
    static_assert(h.count() == 2, "");
    }
#endif

  return;
}
