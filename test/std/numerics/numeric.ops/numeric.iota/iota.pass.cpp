// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_numeric_ops_numeric_iota_iota_73a59038).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <numeric>

// Became constexpr in C++20
// template <class ForwardIterator, class T>
//     void iota(ForwardIterator first, ForwardIterator last, T value);

#include <numeric>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"

namespace libcis_ns_numerics_numeric_ops_numeric_iota_iota_73a59038 { // libcis
template <class InIter>
TEST_CONSTEXPR_CXX20 void
test()
{
    int ia[] = {1, 2, 3, 4, 5};
    int ir[] = {5, 6, 7, 8, 9};
    const unsigned s = sizeof(ia) / sizeof(ia[0]);
    std::iota(InIter(ia), InIter(ia+s), 5);
    for (unsigned i = 0; i < s; ++i)
        assert(ia[i] == ir[i]);
}

TEST_CONSTEXPR_CXX20 bool
test()
{
    test<forward_iterator<int*> >();
    test<bidirectional_iterator<int*> >();
    test<random_access_iterator<int*> >();
    test<int*>();

    return true;
}

int main(int, char**)
{
    test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif
    return 0;

    return 0;
}
} // libcis_ns_numerics_numeric_ops_numeric_iota_iota_73a59038 (libcis)

