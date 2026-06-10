// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_factories_range_repeat_view_iterator_plus).
// main -> test_ranges_range_factories_range_repeat_view_iterator_plus; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// friend constexpr iterator operator+(iterator i, difference_type n);
// friend constexpr iterator operator+(difference_type n, iterator i);

#include <ranges>
#include <cassert>
#include <concepts>

namespace libcis_ns_ranges_range_factories_range_repeat_view_iterator_plus { // libcis: isolate file-scope helpers
constexpr bool test() {
  std::ranges::repeat_view<int> v(10);
  using Iter = std::ranges::iterator_t<std::ranges::repeat_view<int>>;
  auto iter  = v.begin();
  assert(iter + 5 == v.begin() + 5);
  assert(5 + iter == v.begin() + 5);
  assert(2 + iter == v.begin() + 2);
  assert(3 + iter == v.begin() + 3);

  static_assert(std::same_as<decltype(iter + 5), Iter>);
  static_assert(std::same_as<decltype(5 + iter), Iter>);

  return true;
}
} using namespace libcis_ns_ranges_range_factories_range_repeat_view_iterator_plus; // libcis


void test_ranges_range_factories_range_repeat_view_iterator_plus() {
  test();
  static_assert(test());

  return;
}
