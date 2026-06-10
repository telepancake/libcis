// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_factories_range_repeat_view_iterator_minus_eq).
// main -> test_ranges_range_factories_range_repeat_view_iterator_minus_eq; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// constexpr iterator& operator-=(difference_type n);

#include <ranges>
#include <cassert>
#include <concepts>

namespace libcis_ns_ranges_range_factories_range_repeat_view_iterator_minus_eq { // libcis: isolate file-scope helpers
constexpr bool test() {
  using Iter = std::ranges::iterator_t<std::ranges::repeat_view<int>>;
  std::ranges::repeat_view<int> v(10);
  auto iter = v.begin() + 10;
  iter -= 5;
  assert(iter == v.begin() + 5);

  static_assert(std::same_as<decltype(iter -= 5), Iter&>);

  return true;
}
} using namespace libcis_ns_ranges_range_factories_range_repeat_view_iterator_minus_eq; // libcis


void test_ranges_range_factories_range_repeat_view_iterator_minus_eq() {
  test();
  static_assert(test());

  return;
}
