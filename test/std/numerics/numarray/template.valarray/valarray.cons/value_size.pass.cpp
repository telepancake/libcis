// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_numarray_template_valarray_valarray_cons_value_size_d539d7a7).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <valarray>

// template<class T> class valarray;

// valarray(const value_type& x, size_t n);

#include <valarray>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_numarray_template_valarray_valarray_cons_value_size_d539d7a7 { // libcis
int main(int, char**)
{
    {
        std::valarray<int> v(5, 100);
        assert(v.size() == 100);
        for (int i = 0; i < 100; ++i)
            assert(v[i] == 5);
    }
    {
        std::valarray<double> v(2.5, 100);
        assert(v.size() == 100);
        for (int i = 0; i < 100; ++i)
            assert(v[i] == 2.5);
    }
    {
        std::valarray<std::valarray<double> > v(std::valarray<double>(10), 100);
        assert(v.size() == 100);
        for (int i = 0; i < 100; ++i)
            assert(v[i].size() == 10);
    }

  return 0;

    return 0;
}
} // libcis_ns_numerics_numarray_template_valarray_valarray_cons_value_size_d539d7a7 (libcis)

