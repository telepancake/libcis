// AST-transferred from libc++ by tools/transfer.py (slug=utilities_ratio_ratio_comparison_ratio_less_equal).
// main -> test_utilities_ratio_ratio_comparison_ratio_less_equal; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// test ratio_less_equal

#include <ratio>

#include "test_macros.h"

namespace libcis_ns_utilities_ratio_ratio_comparison_ratio_less_equal { // libcis: isolate file-scope helpers
template <class Rat1, class Rat2, bool result>
void test()
{
    static_assert((result == std::ratio_less_equal<Rat1, Rat2>::value), "");
#if TEST_STD_VER > 14
    static_assert((result == std::ratio_less_equal_v<Rat1, Rat2>), "");
#endif
}
} using namespace libcis_ns_utilities_ratio_ratio_comparison_ratio_less_equal; // libcis


void test_utilities_ratio_ratio_comparison_ratio_less_equal()
{
    {
    typedef std::ratio<1, 1> R1;
    typedef std::ratio<1, 1> R2;
    test<R1, R2, true>();
    }
    {
    typedef std::ratio<0x7FFFFFFFFFFFFFFFLL, 1> R1;
    typedef std::ratio<0x7FFFFFFFFFFFFFFFLL, 1> R2;
    test<R1, R2, true>();
    }
    {
    typedef std::ratio<-0x7FFFFFFFFFFFFFFFLL, 1> R1;
    typedef std::ratio<-0x7FFFFFFFFFFFFFFFLL, 1> R2;
    test<R1, R2, true>();
    }
    {
    typedef std::ratio<1, 0x7FFFFFFFFFFFFFFFLL> R1;
    typedef std::ratio<1, 0x7FFFFFFFFFFFFFFFLL> R2;
    test<R1, R2, true>();
    }
    {
    typedef std::ratio<1, 1> R1;
    typedef std::ratio<1, -1> R2;
    test<R1, R2, false>();
    }
    {
    typedef std::ratio<0x7FFFFFFFFFFFFFFFLL, 1> R1;
    typedef std::ratio<-0x7FFFFFFFFFFFFFFFLL, 1> R2;
    test<R1, R2, false>();
    }
    {
    typedef std::ratio<-0x7FFFFFFFFFFFFFFFLL, 1> R1;
    typedef std::ratio<0x7FFFFFFFFFFFFFFFLL, 1> R2;
    test<R1, R2, true>();
    }
    {
    typedef std::ratio<1, 0x7FFFFFFFFFFFFFFFLL> R1;
    typedef std::ratio<1, -0x7FFFFFFFFFFFFFFFLL> R2;
    test<R1, R2, false>();
    }

  return;
}
