// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_lazy_split_range_lazy_split_inner_ctor_outer_iterator).
// main -> test_ranges_range_adaptors_range_lazy_split_range_lazy_split_inner_ctor_outer_iterator; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr explicit inner-iterator::inner-iterator(outer-iterator<Const> i);

#include <ranges>

#include "../types.h"

namespace libcis_ns_ranges_range_adaptors_range_lazy_split_range_lazy_split_inner_ctor_outer_iterator { // libcis: isolate file-scope helpers
static_assert(!std::is_constructible_v<InnerIterNonConst, OuterIterConst>);

template <class Inner, class Outer>
constexpr void test_impl() {
  [[maybe_unused]] Inner i(Outer{});
  // Verify that the constructor is `explicit`.
  static_assert(!std::is_convertible_v<Outer, Inner>);
}

constexpr bool test() {
  test_impl<InnerIterForward, OuterIterForward>();
  test_impl<InnerIterInput, OuterIterInput>();
// Is only constructible if both the outer and the inner iterators have the same constness.
  test_impl<InnerIterConst, OuterIterConst>();
// Note: this works because of an implicit conversion (`OuterIterNonConst` is converted to `OuterIterConst`).
  test_impl<InnerIterConst, OuterIterNonConst>();
  test_impl<InnerIterNonConst, OuterIterNonConst>();

  return true;
}
} using namespace libcis_ns_ranges_range_adaptors_range_lazy_split_range_lazy_split_inner_ctor_outer_iterator; // libcis


void test_ranges_range_adaptors_range_lazy_split_range_lazy_split_inner_ctor_outer_iterator() {
  test();
  static_assert(test());

  return;
}
