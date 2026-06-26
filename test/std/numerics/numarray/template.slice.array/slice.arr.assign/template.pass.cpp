// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_numarray_template_slice_array_slice_arr_assign_template_c31accaa).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <valarray>

// template <class T> class slice_array

// void operator=(const T& value) const;

#include <valarray>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_numarray_template_slice_array_slice_arr_assign_template_c31accaa { // libcis
int main(int, char**)
{
    double a[] = { 0, 0, 0 };
    std::valarray<double> m(a, sizeof(a)/sizeof(a[0]));
    std::slice_array<double> s = m[std::slice(0, 3, 1)];
    s = 42;
    assert(m[0] == 42);
    assert(m[1] == 42);
    assert(m[2] == 42);

    ASSERT_SAME_TYPE(decltype(s = 42), void);

    return 0;

    return 0;
}
} // libcis_ns_numerics_numarray_template_slice_array_slice_arr_assign_template_c31accaa (libcis)

