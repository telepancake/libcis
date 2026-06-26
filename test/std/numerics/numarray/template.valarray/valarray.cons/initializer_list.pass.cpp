// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_numarray_template_valarray_valarray_cons_initializer_list_2140a740).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <valarray>

// template<class T> class valarray;

// valarray(initializer_list<value_type>);

#include <valarray>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_numarray_template_valarray_valarray_cons_initializer_list_2140a740 { // libcis
int main(int, char**)
{
    {
        typedef int T;
        T a[] = {1, 2, 3, 4, 5};
        const unsigned N = sizeof(a)/sizeof(a[0]);
        std::valarray<T> v = {1, 2, 3, 4, 5};
        assert(v.size() == N);
        for (unsigned i = 0; i < N; ++i)
            assert(v[i] == a[i]);
    }
    {
        typedef double T;
        T a[] = {1, 2, 3, 4, 5};
        const unsigned N = sizeof(a)/sizeof(a[0]);
        std::valarray<T> v = {1, 2, 3, 4, 5};
        assert(v.size() == N);
        for (unsigned i = 0; i < N; ++i)
            assert(v[i] == a[i]);
    }

  return 0;

    return 0;
}
} // libcis_ns_numerics_numarray_template_valarray_valarray_cons_initializer_list_2140a740 (libcis)

