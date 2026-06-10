// AST-transferred from libc++ by tools/transfer.py (slug=utilities_utility_pairs_pairs_pair_ctor_const_first_const_second_cxx03).
// main -> test_utilities_utility_pairs_pairs_pair_ctor_const_first_const_second_cxx03; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <utility>

// template <class T1, class T2> struct pair

// pair(const T1& x, const T2& y);

#include <utility>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_utility_pairs_pairs_pair_ctor_const_first_const_second_cxx03 { // libcis: isolate file-scope helpers
class A
{
    int data_;
public:
    A(int data) : data_(data) {}

    bool operator==(const A& a) const {return data_ == a.data_;}
};
} using namespace libcis_ns_utilities_utility_pairs_pairs_pair_ctor_const_first_const_second_cxx03; // libcis


void test_utilities_utility_pairs_pairs_pair_ctor_const_first_const_second_cxx03()
{
    {
        typedef std::pair<float, short*> P;
        P p(3.5f, 0);
        assert(p.first == 3.5f);
        assert(p.second == nullptr);
    }
    {
        typedef std::pair<A, int> P;
        P p(1, 2);
        assert(p.first == A(1));
        assert(p.second == 2);
    }

  return;
}
