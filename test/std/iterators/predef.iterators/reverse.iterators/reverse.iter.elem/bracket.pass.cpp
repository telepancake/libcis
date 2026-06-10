// AST-transferred from libc++ by tools/transfer.py (slug=iterators_predef_iterators_reverse_iterators_reverse_iter_elem_bracket).
// main -> test_iterators_predef_iterators_reverse_iterators_reverse_iter_elem_bracket; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iterator>

// reverse_iterator

// requires RandomAccessIterator<Iter>
// unspecified operator[](difference_type n) const; // constexpr since C++17

#include <iterator>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"

namespace libcis_ns_iterators_predef_iterators_reverse_iterators_reverse_iter_elem_bracket { // libcis: isolate file-scope helpers
template <class It>
TEST_CONSTEXPR_CXX17 void test(It i,
                               typename std::iterator_traits<It>::difference_type n,
                               typename std::iterator_traits<It>::value_type x) {
    typedef typename std::iterator_traits<It>::value_type value_type;
    const std::reverse_iterator<It> r(i);
    value_type rr = r[n];
    assert(rr == x);
}

TEST_CONSTEXPR_CXX17 bool tests() {
    const char* s = "1234567890";
    test(random_access_iterator<const char*>(s+5), 4, '1');
    test(random_access_iterator<const char*>(s+5), 0, '5');
#if TEST_STD_VER >= 20
    test(cpp20_random_access_iterator<const char*>(s + 5), 4, '1');
    test(cpp20_random_access_iterator<const char*>(s + 5), 0, '5');
#endif
    test(s+5, 4, '1');
    test(s+5, 0, '5');
    return true;
}
} using namespace libcis_ns_iterators_predef_iterators_reverse_iterators_reverse_iter_elem_bracket; // libcis


void test_iterators_predef_iterators_reverse_iterators_reverse_iter_elem_bracket() {
    tests();
#if TEST_STD_VER > 14
    static_assert(tests(), "");
#endif
    return;
}
