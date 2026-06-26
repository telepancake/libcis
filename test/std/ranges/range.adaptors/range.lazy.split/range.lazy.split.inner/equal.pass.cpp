// transferred+adapted from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_lazy_split_range_lazy_split_inner_equal_057d8f54).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// friend constexpr bool operator==(const inner-iterator& x, const inner-iterator& y);
//   requires forward_range<Base>;
//
// friend constexpr bool operator==(const inner-iterator& x, default_sentinel_t);

#include <ranges>

#include <concepts>
#include <string_view>

#include "../types.h"

#include "test_range.h"

namespace libcis_ns_ranges_range_adaptors_range_lazy_split_range_lazy_split_inner_equal_057d8f54 { // libcis
constexpr bool test() {
  // When `View` is a forward range, `inner-iterator` supports both overloads of `operator==`.
  {
    SplitViewForward v("abc def", " ");
    auto val = *v.begin();
    auto b = val.begin();
    std::same_as<std::default_sentinel_t> decltype(auto) e = val.end();

    // inner-iterator == inner-iterator
    {
      assert(b == b);
      assert(!(b != b));
    }

    // inner-iterator == default_sentinel
    {
      assert(!(b == e));
      assert(b != e);

      assert(!(b == std::default_sentinel));
      assert(b != std::default_sentinel);
    }
  }

  // When `View` is an input range, `inner-iterator only supports comparing an `inner-iterator` to the default sentinel.
  {
    SplitViewInput v("abc def", ' ');
    auto val = *v.begin();
    auto b = val.begin();
    std::same_as<std::default_sentinel_t> decltype(auto) e = val.end();

    static_assert(!weakly_equality_comparable_with<decltype(b), decltype(b)>);

    assert(!(b == std::default_sentinel));
    assert(b != std::default_sentinel);
    assert(!(b == e));
    assert(b != e);
  }

  return true;
}

int main(int, char**) {
  test();
  static_assert(test());

  return 0;

    return 0;
}
} // libcis_ns_ranges_range_adaptors_range_lazy_split_range_lazy_split_inner_equal_057d8f54 (libcis)

