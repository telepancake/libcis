// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_numarray_template_valarray_valarray_access_const_access_caa4ea25).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <valarray>

// template<class T> class valarray;

// const value_type& operator[](size_t i) const;

#include <valarray>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_numarray_template_valarray_valarray_access_const_access_caa4ea25 { // libcis
int main(int, char**)
{
    {
        typedef int T;
        T a[] = {5, 4, 3, 2, 1};
        const unsigned N = sizeof(a)/sizeof(a[0]);
        const std::valarray<T> v(a, N);
        for (unsigned i = 0; i < N; ++i)
        {
            assert(v[i] == a[i]);
        }
    }

  return 0;

    return 0;
}
} // libcis_ns_numerics_numarray_template_valarray_valarray_access_const_access_caa4ea25 (libcis)

