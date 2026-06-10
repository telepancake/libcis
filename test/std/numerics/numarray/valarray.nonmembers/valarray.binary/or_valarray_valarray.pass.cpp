// AST-transferred from libc++ by tools/transfer.py (slug=numerics_numarray_valarray_nonmembers_valarray_binary_or_valarray_valarray).
// main -> test_numerics_numarray_valarray_nonmembers_valarray_binary_or_valarray_valarray; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <valarray>

// template<class T> class valarray;

// template<class T> valarray<T> operator|(const valarray<T>& x, const valarray<T>& y);

#include <valarray>
#include <cassert>
#include <cstddef>

#include "test_macros.h"

void test_numerics_numarray_valarray_nonmembers_valarray_binary_or_valarray_valarray()
{
    {
        typedef int T;
        T a1[] = {1,  2,  3,  4,  5};
        T a2[] = {6,  7,  8,  9, 10};
        T a3[] = {7,  7, 11, 13, 15};
        const unsigned N = sizeof(a1)/sizeof(a1[0]);
        std::valarray<T> v1(a1, N);
        std::valarray<T> v2(a2, N);
        std::valarray<T> v3 = v1 | v2;
        assert(v1.size() == v2.size());
        assert(v1.size() == v3.size());
        for (std::size_t i = 0; i < v1.size(); ++i)
            assert(v3[i] == a3[i]);
    }

  return;
}
