// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_factories_range_iota_view_sentinel_ctor_value).
// main -> test_ranges_range_factories_range_iota_view_sentinel_ctor_value; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr explicit sentinel(Bound bound);

#include <cassert>
#include <ranges>
#include <type_traits>

#include "test_macros.h"
#include "../types.h"

namespace libcis_ns_ranges_range_factories_range_iota_view_sentinel_ctor_value { // libcis: isolate file-scope helpers
constexpr bool test() {
  {
    using Sent = std::ranges::sentinel_t<std::ranges::iota_view<int, IntSentinelWith<int>>>;
    using Iter = std::ranges::iterator_t<std::ranges::iota_view<int, IntSentinelWith<int>>>;
    auto sent = Sent(IntSentinelWith<int>(42));
    assert(sent == Iter(42));
  }
  {
    using Sent = std::ranges::sentinel_t<std::ranges::iota_view<SomeInt, IntSentinelWith<SomeInt>>>;
    using Iter = std::ranges::iterator_t<std::ranges::iota_view<SomeInt, IntSentinelWith<SomeInt>>>;
    auto sent = Sent(IntSentinelWith<SomeInt>(SomeInt(42)));
    assert(sent == Iter(SomeInt(42)));
  }
  {
    using Sent = std::ranges::sentinel_t<std::ranges::iota_view<SomeInt, IntSentinelWith<SomeInt>>>;
    static_assert(!std::is_convertible_v<Sent, IntSentinelWith<SomeInt>>);
    static_assert( std::is_constructible_v<Sent, IntSentinelWith<SomeInt>>);
  }

  return true;
}
} using namespace libcis_ns_ranges_range_factories_range_iota_view_sentinel_ctor_value; // libcis


void test_ranges_range_factories_range_iota_view_sentinel_ctor_value() {
  test();
  static_assert(test());

  return;
}
