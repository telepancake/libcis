// AST-transferred from libc++ by tools/transfer.py (slug=utilities_utility_pairs_pairs_pair_ctor_pair_U_V_const_ref_cxx03).
// main -> test_utilities_utility_pairs_pairs_pair_ctor_pair_U_V_const_ref_cxx03; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <utility>

// template <class T1, class T2> struct pair

// template <class U, class V> pair(const pair<U, V>& p);

#include <utility>
#include <cassert>

#include "test_macros.h"

void test_utilities_utility_pairs_pairs_pair_ctor_pair_U_V_const_ref_cxx03()
{
    {
        typedef std::pair<int, short> P1;
        typedef std::pair<double, long> P2;
        const P1 p1(3, static_cast<short>(4));
        const P2 p2 = p1;
        assert(p2.first == 3);
        assert(p2.second == 4);
    }

  return;
}
