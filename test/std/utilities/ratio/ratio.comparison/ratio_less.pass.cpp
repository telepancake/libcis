// AST-transferred from libc++ by tools/transfer.py (slug=utilities_ratio_ratio_comparison_ratio_less).
// main -> test_utilities_ratio_ratio_comparison_ratio_less; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// test ratio_less

#include <ratio>

#include "test_macros.h"

namespace libcis_ns_utilities_ratio_ratio_comparison_ratio_less { // libcis: isolate file-scope helpers
template <class Rat1, class Rat2, bool result>
void test()
{
    static_assert((result == std::ratio_less<Rat1, Rat2>::value), "");
#if TEST_STD_VER > 14
    static_assert((result == std::ratio_less_v<Rat1, Rat2>), "");
#endif
}
} using namespace libcis_ns_utilities_ratio_ratio_comparison_ratio_less; // libcis


void test_utilities_ratio_ratio_comparison_ratio_less()
{
    {
    typedef std::ratio<1, 1> R1;
    typedef std::ratio<1, 1> R2;
    test<R1, R2, false>();
    }
    {
    typedef std::ratio<0x7FFFFFFFFFFFFFFFLL, 1> R1;
    typedef std::ratio<0x7FFFFFFFFFFFFFFFLL, 1> R2;
    test<R1, R2, false>();
    }
    {
    typedef std::ratio<-0x7FFFFFFFFFFFFFFFLL, 1> R1;
    typedef std::ratio<-0x7FFFFFFFFFFFFFFFLL, 1> R2;
    test<R1, R2, false>();
    }
    {
    typedef std::ratio<1, 0x7FFFFFFFFFFFFFFFLL> R1;
    typedef std::ratio<1, 0x7FFFFFFFFFFFFFFFLL> R2;
    test<R1, R2, false>();
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
    {
    typedef std::ratio<0x7FFFFFFFFFFFFFFFLL, 0x7FFFFFFFFFFFFFFELL> R1;
    typedef std::ratio<0x7FFFFFFFFFFFFFFDLL, 0x7FFFFFFFFFFFFFFCLL> R2;
    test<R1, R2, true>();
    }
    {
    typedef std::ratio<0x7FFFFFFFFFFFFFFDLL, 0x7FFFFFFFFFFFFFFCLL> R1;
    typedef std::ratio<0x7FFFFFFFFFFFFFFFLL, 0x7FFFFFFFFFFFFFFELL> R2;
    test<R1, R2, false>();
    }
    {
    typedef std::ratio<-0x7FFFFFFFFFFFFFFDLL, 0x7FFFFFFFFFFFFFFCLL> R1;
    typedef std::ratio<-0x7FFFFFFFFFFFFFFFLL, 0x7FFFFFFFFFFFFFFELL> R2;
    test<R1, R2, true>();
    }
    {
    typedef std::ratio<0x7FFFFFFFFFFFFFFFLL, 0x7FFFFFFFFFFFFFFELL> R1;
    typedef std::ratio<0x7FFFFFFFFFFFFFFELL, 0x7FFFFFFFFFFFFFFDLL> R2;
    test<R1, R2, true>();
    }
    {
    typedef std::ratio<641981, 1339063> R1;
    typedef std::ratio<1291640, 2694141LL> R2;
    test<R1, R2, false>();
    }
    {
    typedef std::ratio<1291640, 2694141LL> R1;
    typedef std::ratio<641981, 1339063> R2;
    test<R1, R2, true>();
    }

  return;
}
