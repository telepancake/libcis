// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_numarray_template_valarray_valarray_members_max_f11629a9).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <valarray>

// template<class T> class valarray;

// value_type max() const;

#include <valarray>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_numarray_template_valarray_valarray_members_max_f11629a9 { // libcis
int main(int, char**)
{
    {
        typedef double T;
        T a1[] = {1.5, 2.5, -3, 4, -5.5};
        const unsigned N1 = sizeof(a1)/sizeof(a1[0]);
        std::valarray<T> v1(a1, N1);
        assert(v1.max() == 4.0);
    }
    {
        typedef double T;
        T a1[] = {1.5, 2.5, -3, 4, -5.5};
        const unsigned N1 = sizeof(a1)/sizeof(a1[0]);
        std::valarray<T> v1(a1, N1);
        assert((2*v1).max() == 8.0);
    }

  return 0;

    return 0;
}
} // libcis_ns_numerics_numarray_template_valarray_valarray_members_max_f11629a9 (libcis)

