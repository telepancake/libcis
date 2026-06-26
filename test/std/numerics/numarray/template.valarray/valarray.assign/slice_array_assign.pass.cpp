// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_numarray_template_valarray_valarray_assign_slice_array_assign_46ee3adc).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <valarray>

// template<class T> class valarray;

// valarray& operator=(const slice_array<value_type>& sa);

#include <valarray>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_numarray_template_valarray_valarray_assign_slice_array_assign_46ee3adc { // libcis
int main(int, char**)
{
    int a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    std::valarray<int> v1(a, sizeof(a)/sizeof(a[0]));
    std::valarray<int> v(5);
    v = v1[std::slice(1, 5, 3)];
    assert(v.size() == 5);
    assert(v[0] == 1);
    assert(v[1] == 4);
    assert(v[2] == 7);
    assert(v[3] == 10);
    assert(v[4] == 13);

  return 0;

    return 0;
}
} // libcis_ns_numerics_numarray_template_valarray_valarray_assign_slice_array_assign_46ee3adc (libcis)

