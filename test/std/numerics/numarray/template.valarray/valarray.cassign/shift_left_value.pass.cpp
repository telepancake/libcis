// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_numarray_template_valarray_valarray_cassign_shift_left_value_f488e172).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <valarray>

// template<class T> class valarray;

// valarray& operator<<=(const value_type& x);

#include <valarray>
#include <cassert>
#include <cstddef>

#include "test_macros.h"

namespace libcis_ns_numerics_numarray_template_valarray_valarray_cassign_shift_left_value_f488e172 { // libcis
int main(int, char**)
{
    {
        typedef int T;
        T a1[] = { 1,   2,  3,  4,  5};
        T a2[] = { 8,  16, 24, 32, 40};
        const unsigned N = sizeof(a1)/sizeof(a1[0]);
        std::valarray<T> v1(a1, N);
        std::valarray<T> v2(a2, N);
        v1 <<= 3;
        assert(v1.size() == v2.size());
        for (std::size_t i = 0; i < v1.size(); ++i)
            assert(v1[i] == v2[i]);
    }

  return 0;

    return 0;
}
} // libcis_ns_numerics_numarray_template_valarray_valarray_cassign_shift_left_value_f488e172 (libcis)

