// AST-transferred from libc++ by tools/transfer.py (slug=experimental_utilities_propagate_const_propagate_const_nonmembers_propagate_const_comparison_function_objects_greater_equal).
// main -> test_experimental_utilities_propagate_const_propagate_const_nonmembers_propagate_const_comparison_function_objects_greater_equal; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11

// <propagate_const>

// template <class T> struct greater_equal<experimental::fundamentals_v2::propagate_const<T>>;

#include <experimental/propagate_const>
#include <cassert>
#include <functional>

#include "test_macros.h"
#include "propagate_const_helpers.h"

using std::experimental::propagate_const;

namespace libcis_ns_experimental_utilities_propagate_const_propagate_const_nonmembers_propagate_const_comparison_function_objects_greater_equal { // libcis: isolate file-scope helpers
constexpr bool operator>=(const X &x1, const X &x2) { return x1.i_ >= x2.i_; }
} using namespace libcis_ns_experimental_utilities_propagate_const_propagate_const_nonmembers_propagate_const_comparison_function_objects_greater_equal; // libcis


void test_experimental_utilities_propagate_const_propagate_const_nonmembers_propagate_const_comparison_function_objects_greater_equal() {

  typedef propagate_const<X> P;

  P p1_1(1);
  P p2_1(1);
  P p3_2(2);

  auto c = std::greater_equal<P>();

  assert(c(p1_1,p2_1));
  assert(c(p2_1,p1_1));
  assert(!c(p1_1,p3_2));
  assert(c(p3_2,p1_1));

  return;
}
