// AST-transferred from libc++ by tools/transfer.py (slug=experimental_utilities_propagate_const_propagate_const_class_propagate_const_ctors_convertible_element_type_non_explicit_ctor).
// main -> test_experimental_utilities_propagate_const_propagate_const_class_propagate_const_ctors_convertible_element_type_non_explicit_ctor; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11

// <propagate_const>

// template <class U> constexpr propagate_const(propagate_const<_Up>&& pu);

#include <experimental/propagate_const>
#include "test_macros.h"
#include "propagate_const_helpers.h"
#include <cassert>

using std::experimental::propagate_const;

namespace libcis_ns_experimental_utilities_propagate_const_propagate_const_class_propagate_const_ctors_convertible_element_type_non_explicit_ctor { // libcis: isolate file-scope helpers
typedef propagate_const<CopyConstructibleFromX> P;

void f(const P& p)
{
  assert(*p==2);
}
} using namespace libcis_ns_experimental_utilities_propagate_const_propagate_const_class_propagate_const_ctors_convertible_element_type_non_explicit_ctor; // libcis


void test_experimental_utilities_propagate_const_propagate_const_class_propagate_const_ctors_convertible_element_type_non_explicit_ctor() {
  f(X(2));

  return;
}
