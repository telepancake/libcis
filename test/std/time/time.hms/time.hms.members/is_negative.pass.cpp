// AST-transferred from libc++ by tools/transfer.py (slug=time_time_hms_time_hms_members_is_negative).
// main -> test_time_time_hms_time_hms_members_is_negative; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17
// <chrono>

// template <class Duration>
// class hh_mm_ss
//
// constexpr bool is_negative() const noexcept;

#include <chrono>
#include <cassert>
#include <ratio>
#include <utility>

#include "test_macros.h"

namespace libcis_ns_time_time_hms_time_hms_members_is_negative { // libcis: isolate file-scope helpers
template <typename Duration>
constexpr bool check_neg(Duration d)
{
    ASSERT_SAME_TYPE(bool, decltype(std::declval<std::chrono::hh_mm_ss<Duration>>().is_negative()));
    ASSERT_NOEXCEPT(                std::declval<std::chrono::hh_mm_ss<Duration>>().is_negative());
    return std::chrono::hh_mm_ss<Duration>(d).is_negative();
}
} using namespace libcis_ns_time_time_hms_time_hms_members_is_negative; // libcis


void test_time_time_hms_time_hms_members_is_negative()
{
    using microfortnights = std::chrono::duration<int, std::ratio<756, 625>>;

    static_assert(!check_neg(std::chrono::minutes( 1)), "");
    static_assert( check_neg(std::chrono::minutes(-1)), "");

    assert(!check_neg(std::chrono::seconds( 5000)));
    assert( check_neg(std::chrono::seconds(-5000)));
    assert(!check_neg(std::chrono::minutes( 5000)));
    assert( check_neg(std::chrono::minutes(-5000)));
    assert(!check_neg(std::chrono::hours( 11)));
    assert( check_neg(std::chrono::hours(-11)));

    assert(!check_neg(std::chrono::milliseconds( 123456789LL)));
    assert( check_neg(std::chrono::milliseconds(-123456789LL)));
    assert(!check_neg(std::chrono::microseconds( 123456789LL)));
    assert( check_neg(std::chrono::microseconds(-123456789LL)));
    assert(!check_neg(std::chrono::nanoseconds( 123456789LL)));
    assert( check_neg(std::chrono::nanoseconds(-123456789LL)));

    assert(!check_neg(microfortnights( 10000)));
    assert( check_neg(microfortnights(-10000)));

    return;
}
