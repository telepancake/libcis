// AST-transferred from libc++ by tools/transfer.py (slug=numerics_numarray_template_valarray_valarray_members_size).
// main -> test_numerics_numarray_template_valarray_valarray_members_size; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <valarray>

// template<class T> class valarray;

// size_t size() const;

#include <valarray>
#include <cassert>

#include "test_macros.h"

void test_numerics_numarray_template_valarray_valarray_members_size()
{
    {
        typedef int T;
        T a1[] = {1, 2, 3, 4, 5};
        const unsigned N1 = sizeof(a1)/sizeof(a1[0]);
        std::valarray<T> v1(a1, N1);
        assert(v1.size() == N1);
    }
    {
        typedef int T;
        T a1[] = {1, 2, 3, 4, 5};
        const unsigned N1 = 0;
        std::valarray<T> v1(a1, N1);
        assert(v1.size() == N1);
    }
    {
        typedef int T;
        const unsigned N1 = 0;
        std::valarray<T> v1;
        assert(v1.size() == N1);
    }

  return;
}
