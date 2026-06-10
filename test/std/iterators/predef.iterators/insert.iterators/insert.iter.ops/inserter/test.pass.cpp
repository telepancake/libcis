// AST-transferred from libc++ by tools/transfer.py (slug=iterators_predef_iterators_insert_iterators_insert_iter_ops_inserter_test).
// main -> test_iterators_predef_iterators_insert_iterators_insert_iter_ops_inserter_test; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iterator>

// template <InsertionContainer Cont>
//   insert_iterator<Cont>
//   inserter(Cont& x, Cont::iterator i);

#include <cassert>
#include <iterator>
#include <vector>

#include "test_macros.h"
#include "nasty_containers.h"
#include "test_constexpr_container.h"

namespace libcis_ns_iterators_predef_iterators_insert_iterators_insert_iter_ops_inserter_test { // libcis: isolate file-scope helpers
template <class C>
TEST_CONSTEXPR_CXX20 bool
test(C c)
{
    std::insert_iterator<C> i = std::inserter(c, c.end());
    i = 0;
    assert(c.size() == 1);
    assert(c.back() == 0);
    return true;
}
} using namespace libcis_ns_iterators_predef_iterators_insert_iterators_insert_iter_ops_inserter_test; // libcis


void test_iterators_predef_iterators_insert_iterators_insert_iter_ops_inserter_test()
{
    test(std::vector<int>());
    test(nasty_vector<int>());
#if TEST_STD_VER >= 20
    test(ConstexprFixedCapacityDeque<int, 10>());
    static_assert(test(ConstexprFixedCapacityDeque<int, 10>()));
#endif
    return;
}
