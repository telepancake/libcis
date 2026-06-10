// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_factories_range_repeat_view_iterator_subscript).
// main -> test_ranges_range_factories_range_repeat_view_iterator_subscript; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// constexpr const W & operator[](difference_type n) const noexcept;

#include <ranges>
#include <cassert>
#include <concepts>
#include <algorithm>

namespace libcis_ns_ranges_range_factories_range_repeat_view_iterator_subscript { // libcis: isolate file-scope helpers
constexpr bool test() {
  // unbound
  {
    std::ranges::repeat_view<int> v(31);
    auto iter = v.begin();
    assert(std::ranges::all_of(std::views::iota(0, 100), [&v](int i) { return v[i] == 31; }));

    static_assert(noexcept(iter[0]));
    static_assert(std::same_as<decltype(iter[0]), const int&>);
  }

  // bound
  {
    std::ranges::repeat_view<int, int> v(32);
    auto iter = v.begin();
    assert(std::ranges::all_of(v, [](int i) { return i == 32; }));
    static_assert(noexcept(iter[0]));
    static_assert(std::same_as<decltype(iter[0]), const int&>);
  }
  return true;
}
} using namespace libcis_ns_ranges_range_factories_range_repeat_view_iterator_subscript; // libcis


void test_ranges_range_factories_range_repeat_view_iterator_subscript() {
  test();
  static_assert(test());

  return;
}
