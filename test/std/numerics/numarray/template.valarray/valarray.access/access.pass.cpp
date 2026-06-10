// AST-transferred from libc++ by tools/transfer.py (slug=numerics_numarray_template_valarray_valarray_access_access).
// main -> test_numerics_numarray_template_valarray_valarray_access_access; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <valarray>

// template<class T> class valarray;

// value_type& operator[](size_t i);

#include <valarray>
#include <cassert>

#include "test_macros.h"

void test_numerics_numarray_template_valarray_valarray_access_access()
{
    {
        typedef int T;
        T a[] = {5, 4, 3, 2, 1};
        const unsigned N = sizeof(a)/sizeof(a[0]);
        std::valarray<T> v(a, N);
        for (unsigned i = 0; i < N; ++i)
        {
            assert(v[i] == a[i]);
            v[i] = i;
            assert(v[i] == static_cast<int>(i));
        }
    }

  return;
}
