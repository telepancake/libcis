// AST-transferred from libc++ by tools/transfer.py (slug=utilities_function_objects_refwrap_refwrap_comparisons_equal_refwrap_refwrap_const).
// main -> test_utilities_function_objects_refwrap_refwrap_comparisons_equal_refwrap_refwrap_const; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// REQUIRES: std-at-least-c++26

// <functional>

// class reference_wrapper

// [refwrap.comparisons], comparisons

// friend constexpr bool operator==(reference_wrapper, reference_wrapper<const T>);                       // Since C++26

#include <cassert>
#include <concepts>
#include <functional>

#include "test_comparisons.h"
#include "test_macros.h"

// Test SFINAE.

namespace libcis_ns_utilities_function_objects_refwrap_refwrap_comparisons_equal_refwrap_refwrap_const { // libcis: isolate file-scope helpers
static_assert(
    HasOperatorEqual<std::reference_wrapper<EqualityComparable>, std::reference_wrapper<const EqualityComparable>>);

static_assert(
    !HasOperatorEqual<std::reference_wrapper<EqualityComparable>, std::reference_wrapper<const NonComparable>>);

// Test equality.

template <typename T>
constexpr void test() {
  T i{92};
  T j{84};

  std::reference_wrapper<T> rw1{i};

  std::reference_wrapper<T> rw3{j};
  std::reference_wrapper<const T> crw1{i};
  std::reference_wrapper<const T> crw3{j};

  AssertEqualityReturnBool<decltype(rw1), decltype(crw1)>();
  assert(testEquality(rw1, crw1, true));
  assert(testEquality(rw1, crw3, false));
}

constexpr bool test() {
  test<int>();
  test<EqualityComparable>();

  return true;
}
} using namespace libcis_ns_utilities_function_objects_refwrap_refwrap_comparisons_equal_refwrap_refwrap_const; // libcis


void test_utilities_function_objects_refwrap_refwrap_comparisons_equal_refwrap_refwrap_const() {
  test();
  static_assert(test());

  return;
}
