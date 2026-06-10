// AST-transferred from libc++ by tools/transfer.py (slug=iterators_predef_iterators_insert_iterators_insert_iter_ops_insert_iter_op_post).
// main -> test_iterators_predef_iterators_insert_iterators_insert_iter_ops_insert_iter_op_post; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iterator>

// insert_iterator

// insert_iterator<Cont> operator++(int);

#include <cassert>
#include <iterator>
#include <vector>

#include "test_macros.h"
#include "nasty_containers.h"
#include "test_constexpr_container.h"

namespace libcis_ns_iterators_predef_iterators_insert_iterators_insert_iter_ops_insert_iter_op_post { // libcis: isolate file-scope helpers
template <class C>
TEST_CONSTEXPR_CXX20 bool
test(C c)
{
    std::insert_iterator<C> i(c, c.end());
    std::insert_iterator<C> r = i++;
    r = 0;
    assert(c.size() == 1);
    assert(c.back() == 0);
    return true;
}
} using namespace libcis_ns_iterators_predef_iterators_insert_iterators_insert_iter_ops_insert_iter_op_post; // libcis


void test_iterators_predef_iterators_insert_iterators_insert_iter_ops_insert_iter_op_post()
{
    test(std::vector<int>());
    test(nasty_vector<int>());
#if TEST_STD_VER >= 20
    test(ConstexprFixedCapacityDeque<int, 10>());
    static_assert(test(ConstexprFixedCapacityDeque<int, 10>()));
#endif
    return;
}
