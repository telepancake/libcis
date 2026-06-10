// AST-transferred from libc++ by tools/transfer.py (slug=time_time_duration_time_duration_special_min).
// main -> test_time_time_duration_time_duration_special_min; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <chrono>

// duration

// static constexpr duration min(); // noexcept after C++17

#include <chrono>
#include <limits>
#include <cassert>

#include "test_macros.h"
#include "../../rep.h"

namespace libcis_ns_time_time_duration_time_duration_special_min { // libcis: isolate file-scope helpers
template <class D>
void test()
{
    LIBCPP_ASSERT_NOEXCEPT(std::chrono::duration_values<typename D::rep>::min());
#if TEST_STD_VER > 17
    ASSERT_NOEXCEPT(       std::chrono::duration_values<typename D::rep>::min());
#endif
    {
    typedef typename D::rep DRep;
    DRep min_rep = std::chrono::duration_values<DRep>::min();
    assert(D::min().count() == min_rep);
    }
#if TEST_STD_VER >= 11
    {
    typedef typename D::rep DRep;
    constexpr DRep min_rep = std::chrono::duration_values<DRep>::min();
    static_assert(D::min().count() == min_rep, "");
    }
#endif
}
} using namespace libcis_ns_time_time_duration_time_duration_special_min; // libcis


void test_time_time_duration_time_duration_special_min()
{
    test<std::chrono::duration<int> >();
    test<std::chrono::duration<Rep> >();

  return;
}
