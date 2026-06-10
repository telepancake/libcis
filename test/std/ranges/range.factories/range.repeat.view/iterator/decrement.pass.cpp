// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_factories_range_repeat_view_iterator_decrement).
// main -> test_ranges_range_factories_range_repeat_view_iterator_decrement; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// constexpr iterator& operator--();
// constexpr iterator operator--(int);

#include <ranges>
#include <cassert>

namespace libcis_ns_ranges_range_factories_range_repeat_view_iterator_decrement { // libcis: isolate file-scope helpers
constexpr bool test() {
  using Iter = std::ranges::iterator_t<std::ranges::repeat_view<int>>;
  std::ranges::repeat_view<int> rv(10);
  auto iter = rv.begin() + 10;

  assert(iter-- == rv.begin() + 10);
  assert(--iter == rv.begin() + 8);

  static_assert(std::same_as<decltype(iter--), Iter>);
  static_assert(std::same_as<decltype(--iter), Iter&>);

  return true;
}
} using namespace libcis_ns_ranges_range_factories_range_repeat_view_iterator_decrement; // libcis


void test_ranges_range_factories_range_repeat_view_iterator_decrement() {
  test();
  static_assert(test());

  return;
}
