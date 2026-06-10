// AST-transferred from libc++ by tools/transfer.py (slug=numerics_numeric_ops_accumulate_accumulate).
// main -> test_numerics_numeric_ops_accumulate_accumulate; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <numeric>

// Became constexpr in C++20
// template <InputIterator Iter, MoveConstructible T>
//   requires HasPlus<T, Iter::reference>
//         && HasAssign<T, HasPlus<T, Iter::reference>::result_type>
//   T
//   accumulate(Iter first, Iter last, T init);

#include <numeric>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"

namespace { // libcis: isolate file-scope helpers
template <class Iter, class T>
TEST_CONSTEXPR_CXX20 void
test(Iter first, Iter last, T init, T x)
{
    assert(std::accumulate(first, last, init) == x);
}

template <class Iter>
TEST_CONSTEXPR_CXX20 void
test()
{
    int ia[] = {1, 2, 3, 4, 5, 6};
    unsigned sa = sizeof(ia) / sizeof(ia[0]);
    test(Iter(ia), Iter(ia), 0, 0);
    test(Iter(ia), Iter(ia), 10, 10);
    test(Iter(ia), Iter(ia+1), 0, 1);
    test(Iter(ia), Iter(ia+1), 10, 11);
    test(Iter(ia), Iter(ia+2), 0, 3);
    test(Iter(ia), Iter(ia+2), 10, 13);
    test(Iter(ia), Iter(ia+sa), 0, 21);
    test(Iter(ia), Iter(ia+sa), 10, 31);
}

TEST_CONSTEXPR_CXX20 bool
test()
{
    test<cpp17_input_iterator<const int*> >();
    test<forward_iterator<const int*> >();
    test<bidirectional_iterator<const int*> >();
    test<random_access_iterator<const int*> >();
    test<const int*>();

    return true;
}
} // anonymous namespace (libcis)


void test_numerics_numeric_ops_accumulate_accumulate()
{
    test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif
    return;
}
