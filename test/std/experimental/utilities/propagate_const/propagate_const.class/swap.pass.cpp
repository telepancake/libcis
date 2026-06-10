// AST-transferred from libc++ by tools/transfer.py (slug=experimental_utilities_propagate_const_propagate_const_class_swap).
// main -> test_experimental_utilities_propagate_const_propagate_const_class_swap; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11

// <propagate_const>

// template <class T> constexpr void propagate_const::swap(propagate_const<T>& x);

#include <experimental/propagate_const>
#include "test_macros.h"
#include "propagate_const_helpers.h"
#include <cassert>

using std::experimental::propagate_const;

namespace libcis_ns_experimental_utilities_propagate_const_propagate_const_class_swap { // libcis: isolate file-scope helpers
bool swap_called = false;
void swap(X &, X &) { swap_called = true; }
} using namespace libcis_ns_experimental_utilities_propagate_const_propagate_const_class_swap; // libcis


void test_experimental_utilities_propagate_const_propagate_const_class_swap() {
  typedef propagate_const<X> P;
  P p1(1);
  P p2(2);
  p1.swap(p2);
  assert(swap_called);

  return;
}
