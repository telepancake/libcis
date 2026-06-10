// AST-transferred from libc++ by tools/transfer.py (slug=numerics_numarray_template_valarray_valarray_assign_value_assign).
// main -> test_numerics_numarray_template_valarray_valarray_assign_value_assign; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <valarray>

// template<class T> class valarray;

// valarray& operator=(const value_type& x);

#include <valarray>
#include <cassert>
#include <cstddef>

#include "test_macros.h"

void test_numerics_numarray_template_valarray_valarray_assign_value_assign()
{
    {
        typedef int T;
        T a[] = {1, 2, 3, 4, 5};
        const unsigned N = sizeof(a)/sizeof(a[0]);
        std::valarray<T> v(a, N);
        v = 7;
        assert(v.size() == N);
        for (std::size_t i = 0; i < v.size(); ++i)
            assert(v[i] == 7);
    }

  return;
}
