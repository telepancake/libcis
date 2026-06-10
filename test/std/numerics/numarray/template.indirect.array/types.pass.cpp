// AST-transferred from libc++ by tools/transfer.py (slug=numerics_numarray_template_indirect_array_types).
// main -> test_numerics_numarray_template_indirect_array_types; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <valarray>

// template <class T>
// class indirect_array
// {
// public:
//     typedef T value_type;

#include <valarray>
#include <type_traits>

#include "test_macros.h"

void test_numerics_numarray_template_indirect_array_types()
{
    static_assert((std::is_same<std::indirect_array<int>::value_type, int>::value), "");

  return;
}
