// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_lazy_split_range_lazy_split_outer_equal).
// main -> test_ranges_range_adaptors_range_lazy_split_range_lazy_split_outer_equal; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// friend constexpr bool operator==(const outer-iterator& x, const outer-iterator& y)
//   requires forward_range<Base>;
//
// friend constexpr bool operator==(const outer-iterator& x, default_sentinel_t);

#include <ranges>

#include <cassert>
#include <concepts>
#include <string_view>

#include "../types.h"

#include "test_range.h"

namespace libcis_ns_ranges_range_adaptors_range_lazy_split_range_lazy_split_outer_equal { // libcis: isolate file-scope helpers
constexpr bool test() {
  // Forward range supports both overloads of `operator==`.
  {
    // outer-iterator == outer-iterator
    {
      SplitViewForward v("abc def", " ");
      auto b = v.begin(), e = v.end();

      assert(b == b);
      assert(!(b != b));

      assert(e == e);
      assert(!(e != e));

      assert(!(b == e));
      assert(b != e);
    }

    // outer-iterator == default_sentinel
    {
      SplitViewForward v("abc def", " ");
      auto b = v.begin(), e = v.end();

      assert(!(b == std::default_sentinel));
      assert(b != std::default_sentinel);
      assert(e == std::default_sentinel);
      assert(!(e != std::default_sentinel));
    }

      // Default-constructed `outer-iterator`s compare equal.
      {
        OuterIterForward i1, i2;
        assert(i1 == i2);
        assert(!(i1 != i2));
      }
  }

  // Input range only supports comparing an `outer-iterator` to the default sentinel.
  {
    using namespace std::string_view_literals;
    SplitViewInput v("abc def"sv, ' ');
    auto b = v.begin();
    std::same_as<std::default_sentinel_t> decltype(auto) e = v.end();

    static_assert(!weakly_equality_comparable_with<decltype(b), decltype(b)>);

    assert(!(b == std::default_sentinel));
    assert(b != std::default_sentinel);
    assert(!(b == e));
    assert(b != e);
  }

  return true;
}
} using namespace libcis_ns_ranges_range_adaptors_range_lazy_split_range_lazy_split_outer_equal; // libcis


void test_ranges_range_adaptors_range_lazy_split_range_lazy_split_outer_equal() {
  test();
  static_assert(test());

  return;
}
