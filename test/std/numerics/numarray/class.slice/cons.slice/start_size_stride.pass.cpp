// AST-transferred from libc++ by tools/transfer.py (slug=numerics_numarray_class_slice_cons_slice_start_size_stride).
// main -> test_numerics_numarray_class_slice_cons_slice_start_size_stride; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <valarray>

// class slice;

// slice(size_t start, size_t size, size_t stride);

#include <valarray>
#include <cassert>

#include "test_macros.h"

void test_numerics_numarray_class_slice_cons_slice_start_size_stride()
{
    std::slice s(1, 3, 2);
    assert(s.start() == 1);
    assert(s.size() == 3);
    assert(s.stride() == 2);

  return;
}
