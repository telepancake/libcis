// AST-transferred from libc++ by tools/transfer.py (slug=numerics_numarray_class_slice_cons_slice_default).
// main -> test_numerics_numarray_class_slice_cons_slice_default; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <valarray>

// class slice;

// slice();

#include <valarray>
#include <cassert>

#include "test_macros.h"

void test_numerics_numarray_class_slice_cons_slice_default()
{
    std::slice s;
    assert(s.start() == 0);
    assert(s.size() == 0);
    assert(s.stride() == 0);

  return;
}
