// AST-transferred from libc++ by tools/transfer.py (slug=iterators_predef_iterators_move_iterators_move_iter_ops_move_iter_op_const_convert).
// main -> test_iterators_predef_iterators_move_iterators_move_iter_ops_move_iter_op_const_convert; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iterator>

// move_iterator

// template <class U>
//   requires HasConstructor<Iter, const U&>
//   move_iterator(const move_iterator<U> &u);
//
//  constexpr in C++17

#include <iterator>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"

namespace libcis_ns_iterators_predef_iterators_move_iterators_move_iter_ops_move_iter_op_const_convert { // libcis: isolate file-scope helpers
template <class It, class U>
void
test(U u)
{
    const std::move_iterator<U> r2(u);
    std::move_iterator<It> r1 = r2;
    assert(base(r1.base()) == base(u));
}

struct Base {};
struct Derived : Base {};
} using namespace libcis_ns_iterators_predef_iterators_move_iterators_move_iter_ops_move_iter_op_const_convert; // libcis


void test_iterators_predef_iterators_move_iterators_move_iter_ops_move_iter_op_const_convert()
{
    Derived d;

    test<cpp17_input_iterator<Base*> >(cpp17_input_iterator<Derived*>(&d));
    test<forward_iterator<Base*> >(forward_iterator<Derived*>(&d));
    test<bidirectional_iterator<Base*> >(bidirectional_iterator<Derived*>(&d));
    test<random_access_iterator<const Base*> >(random_access_iterator<Derived*>(&d));
    test<Base*>(&d);

#if TEST_STD_VER > 14
    {
    constexpr const Derived *p = nullptr;
    constexpr std::move_iterator<const Derived *>     it1 = std::make_move_iterator(p);
    constexpr std::move_iterator<const Base *>        it2(it1);
    static_assert(it2.base() == p);
    }
#endif

  return;
}
