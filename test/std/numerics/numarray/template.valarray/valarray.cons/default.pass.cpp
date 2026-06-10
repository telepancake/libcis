// AST-transferred from libc++ by tools/transfer.py (slug=numerics_numarray_template_valarray_valarray_cons_default).
// main -> test_numerics_numarray_template_valarray_valarray_cons_default; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <valarray>

// template<class T> class valarray;

// valarray();

#include <valarray>
#include <cassert>

#include "test_macros.h"

namespace { // libcis: isolate file-scope helpers
struct S {
    S() { ctor_called = true; }
    static bool ctor_called;
};
} // anonymous namespace (libcis)


bool S::ctor_called = false;

void test_numerics_numarray_template_valarray_valarray_cons_default()
{
    {
        std::valarray<int> v;
        assert(v.size() == 0);
    }
    {
        std::valarray<float> v;
        assert(v.size() == 0);
    }
    {
        std::valarray<double> v;
        assert(v.size() == 0);
    }
    {
        std::valarray<std::valarray<double> > v;
        assert(v.size() == 0);
    }
    {
        std::valarray<S> v;
        assert(v.size() == 0);
        assert(!S::ctor_called);
    }

  return;
}
