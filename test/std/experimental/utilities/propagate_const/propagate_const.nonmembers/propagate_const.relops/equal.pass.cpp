// AST-transferred from libc++ by tools/transfer.py (slug=experimental_utilities_propagate_const_propagate_const_nonmembers_propagate_const_relops_equal).
// main -> test_experimental_utilities_propagate_const_propagate_const_nonmembers_propagate_const_relops_equal; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11

// <propagate_const>

// template <class T> constexpr bool operator==(const propagate_const<T>& x, const propagate_const<T>& y);
// template <class T> constexpr bool operator==(const T& x, const propagate_const<T>& y);
// template <class T> constexpr bool operator==(const propagate_const<T>& x, const T& y);

#include <experimental/propagate_const>
#include <cassert>
#include <cstddef>

#include "test_macros.h"
#include "propagate_const_helpers.h"

using std::experimental::propagate_const;
using std::nullptr_t;

namespace libcis_ns_experimental_utilities_propagate_const_propagate_const_nonmembers_propagate_const_relops_equal { // libcis: isolate file-scope helpers
constexpr bool operator==(const X &lhs, const X &rhs) {
  return lhs.i_ == rhs.i_;
}

constexpr bool operator==(const X &, const nullptr_t &) {
  return false;
}

constexpr bool operator==(const nullptr_t &, const X &) {
  return false;
}
} using namespace libcis_ns_experimental_utilities_propagate_const_propagate_const_nonmembers_propagate_const_relops_equal; // libcis


void test_experimental_utilities_propagate_const_propagate_const_nonmembers_propagate_const_relops_equal() {
  constexpr X x1_1(1);
  constexpr X x2_1(1);
  constexpr X x3_2(2);

  static_assert(x1_1 == x1_1, "");
  static_assert(x1_1 == x2_1, "");
  static_assert(!(x1_1 == x3_2), "");

  typedef propagate_const<X> P;

  constexpr P p1_1(1);
  constexpr P p2_1(1);
  constexpr P p3_2(2);

  static_assert(p1_1 == p1_1, "");
  static_assert(p1_1 == p2_1, "");
  static_assert(!(p1_1 == p3_2), "");

  static_assert(x1_1 == p1_1, "");
  static_assert(!(x1_1 == p3_2), "");

  static_assert(p1_1 == x1_1, "");
  static_assert(!(p1_1 == x3_2), "");

  static_assert(!(p1_1==nullptr),"");
  static_assert(!(nullptr==p1_1),"");

  return;
}
