// AST-transferred from libc++ by tools/transfer.py (slug=iterators_predef_iterators_reverse_iterators_reverse_iter_cons_ctor_default).
// main -> test_iterators_predef_iterators_reverse_iterators_reverse_iter_cons_ctor_default; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iterator>

// reverse_iterator

// reverse_iterator(); // constexpr since C++17

#include <iterator>

#include "test_macros.h"
#include "test_iterators.h"

namespace libcis_ns_iterators_predef_iterators_reverse_iterators_reverse_iter_cons_ctor_default { // libcis: isolate file-scope helpers
template <class It>
TEST_CONSTEXPR_CXX17 void test() {
    std::reverse_iterator<It> r;
    (void)r;
}

TEST_CONSTEXPR_CXX17 bool tests() {
    test<bidirectional_iterator<const char*> >();
    test<random_access_iterator<char*> >();
#if TEST_STD_VER >= 20
    test<cpp20_random_access_iterator<char*> >();
#endif
    test<char*>();
    test<const char*>();
    return true;
}
} using namespace libcis_ns_iterators_predef_iterators_reverse_iterators_reverse_iter_cons_ctor_default; // libcis


void test_iterators_predef_iterators_reverse_iterators_reverse_iter_cons_ctor_default() {
    tests();
#if TEST_STD_VER > 14
    static_assert(tests(), "");
#endif
    return;
}
