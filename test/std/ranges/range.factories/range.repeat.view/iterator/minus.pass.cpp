// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_factories_range_repeat_view_iterator_minus).
// main -> test_ranges_range_factories_range_repeat_view_iterator_minus; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// friend constexpr iterator operator-(iterator i, difference_type n);
// friend constexpr difference_type operator-(const iterator& x, const iterator& y);

#include <cassert>
#include <concepts>
#include <cstddef>
#include <ranges>

namespace libcis_ns_ranges_range_factories_range_repeat_view_iterator_minus { // libcis: isolate file-scope helpers
constexpr bool test() {
  // <iterator> - difference_type
  {
    using Iter = std::ranges::iterator_t<std::ranges::repeat_view<int>>;
    std::ranges::repeat_view<int> v(0);
    Iter iter = v.begin() + 10;
    assert(iter - 5 == v.begin() + 5);
    static_assert(std::same_as<decltype(iter - 5), Iter>);
  }

  // <iterator> - <iterator>
  {
    // unbound
    {
      std::ranges::repeat_view<int> v(0);
      auto iter1 = v.begin() + 10;
      auto iter2 = v.begin() + 5;
      assert(iter1 - iter2 == 5);
      static_assert(std::same_as<decltype(iter1 - iter2), ptrdiff_t>);
    }

    // bound && signed bound sentinel
    {
      std::ranges::repeat_view<int, int> v(0, 20);
      auto iter1 = v.begin() + 10;
      auto iter2 = v.begin() + 5;
      assert(iter1 - iter2 == 5);
      static_assert(std::same_as<decltype(iter1 - iter2), int>);
    }

    // bound && unsigned bound sentinel
    {
      std::ranges::repeat_view<int, unsigned> v(0, 20);
      auto iter1 = v.begin() + 10;
      auto iter2 = v.begin() + 5;
      assert(iter1 - iter2 == 5);
      static_assert(sizeof(decltype(iter1 - iter2)) > sizeof(unsigned));
    }
  }

  return true;
}
} using namespace libcis_ns_ranges_range_factories_range_repeat_view_iterator_minus; // libcis


void test_ranges_range_factories_range_repeat_view_iterator_minus() {
  test();
  static_assert(test());

  return;
}
