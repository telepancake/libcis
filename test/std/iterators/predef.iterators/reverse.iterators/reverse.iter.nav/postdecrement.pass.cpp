// AST-transferred from libc++ by tools/transfer.py (slug=iterators_predef_iterators_reverse_iterators_reverse_iter_nav_postdecrement).
// main -> test_iterators_predef_iterators_reverse_iterators_reverse_iter_nav_postdecrement; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iterator>

// reverse_iterator

// reverse_iterator operator--(int); // constexpr in C++17

#include <iterator>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"

namespace libcis_ns_iterators_predef_iterators_reverse_iterators_reverse_iter_nav_postdecrement { // libcis: isolate file-scope helpers
template <class It>
TEST_CONSTEXPR_CXX17 void test(It i, It x) {
    std::reverse_iterator<It> r(i);
    std::reverse_iterator<It> rr = r--;
    assert(r.base() == x);
    assert(rr.base() == i);
}

TEST_CONSTEXPR_CXX17 bool tests() {
    const char* s = "123";
    test(bidirectional_iterator<const char*>(s+1), bidirectional_iterator<const char*>(s+2));
    test(random_access_iterator<const char*>(s+1), random_access_iterator<const char*>(s+2));
#if TEST_STD_VER >= 20
    test(cpp20_random_access_iterator<const char*>(s + 1), cpp20_random_access_iterator<const char*>(s + 2));
#endif
    test(s+1, s+2);
    return true;
}
} using namespace libcis_ns_iterators_predef_iterators_reverse_iterators_reverse_iter_nav_postdecrement; // libcis


void test_iterators_predef_iterators_reverse_iterators_reverse_iter_nav_postdecrement() {
    tests();
#if TEST_STD_VER > 14
    static_assert(tests(), "");
#endif
    return;
}
