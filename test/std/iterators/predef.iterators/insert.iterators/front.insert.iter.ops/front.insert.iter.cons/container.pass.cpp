// AST-transferred from libc++ by tools/transfer.py (slug=iterators_predef_iterators_insert_iterators_front_insert_iter_ops_front_insert_iter_cons_container).
// main -> test_iterators_predef_iterators_insert_iterators_front_insert_iter_ops_front_insert_iter_cons_container; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iterator>

// front_insert_iterator

// explicit front_insert_iterator(Cont& x); // constexpr in C++20

#include <iterator>
#include <list>

#include "test_macros.h"
#include "nasty_containers.h"
#include "test_constexpr_container.h"

namespace libcis_ns_iterators_predef_iterators_insert_iterators_front_insert_iter_ops_front_insert_iter_cons_container { // libcis: isolate file-scope helpers
template <class C>
TEST_CONSTEXPR_CXX20 bool
test(C c)
{
    std::front_insert_iterator<C> i(c);
    return true;
}
} using namespace libcis_ns_iterators_predef_iterators_insert_iterators_front_insert_iter_ops_front_insert_iter_cons_container; // libcis


void test_iterators_predef_iterators_insert_iterators_front_insert_iter_ops_front_insert_iter_cons_container()
{
    test(std::list<int>());
    test(nasty_list<int>());
#if TEST_STD_VER >= 20
    test(ConstexprFixedCapacityDeque<int, 10>());
    static_assert(test(ConstexprFixedCapacityDeque<int, 10>()));
#endif
    return;
}
