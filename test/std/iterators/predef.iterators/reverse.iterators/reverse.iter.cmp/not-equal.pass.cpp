// AST-transferred from libc++ by tools/transfer.py (slug=iterators_predef_iterators_reverse_iterators_reverse_iter_cmp_not_equal).
// main -> test_iterators_predef_iterators_reverse_iterators_reverse_iter_cmp_not_equal; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iterator>

// reverse_iterator

// template <BidirectionalIterator Iter1, BidirectionalIterator Iter2>
//   requires HasEqualTo<Iter1, Iter2>
// bool operator!=(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y); // constexpr in C++17

#include <iterator>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"

namespace libcis_ns_iterators_predef_iterators_reverse_iterators_reverse_iter_cmp_not_equal { // libcis: isolate file-scope helpers
template <class It>
TEST_CONSTEXPR_CXX17 void test(It l, It r, bool x) {
    const std::reverse_iterator<It> r1(l);
    const std::reverse_iterator<It> r2(r);
    assert((r1 != r2) == x);
}

TEST_CONSTEXPR_CXX17 bool tests() {
    const char* s = "1234567890";
    test(bidirectional_iterator<const char*>(s), bidirectional_iterator<const char*>(s), false);
    test(bidirectional_iterator<const char*>(s), bidirectional_iterator<const char*>(s+1), true);
    test(random_access_iterator<const char*>(s), random_access_iterator<const char*>(s), false);
    test(random_access_iterator<const char*>(s), random_access_iterator<const char*>(s+1), true);
#if TEST_STD_VER >= 20
    test(cpp20_random_access_iterator<const char*>(s), cpp20_random_access_iterator<const char*>(s), false);
    test(cpp20_random_access_iterator<const char*>(s), cpp20_random_access_iterator<const char*>(s + 1), true);
#endif
    test(s, s, false);
    test(s, s+1, true);
    return true;
}
} using namespace libcis_ns_iterators_predef_iterators_reverse_iterators_reverse_iter_cmp_not_equal; // libcis


void test_iterators_predef_iterators_reverse_iterators_reverse_iter_cmp_not_equal() {
    tests();
#if TEST_STD_VER > 14
    static_assert(tests(), "");
#endif
    return;
}
