// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_lazy_split_range_lazy_split_outer_value_end).
// main -> test_ranges_range_adaptors_range_lazy_split_range_lazy_split_outer_value_end; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// std::ranges::lazy_split_view::outer-iterator::value_type::end()

#include <ranges>

#include <cassert>
#include "../types.h"

namespace libcis_ns_ranges_range_adaptors_range_lazy_split_range_lazy_split_outer_value_end { // libcis: isolate file-scope helpers
constexpr bool test() {
  // `View` is a forward range.
  {
    CopyableView input("a");

    // Non-const.
    {
      SplitViewCopyable v(input, "b");
      auto val = *v.begin();

      static_assert(std::same_as<decltype(val.end()), std::default_sentinel_t>);
      static_assert(noexcept(val.end()));
      [[maybe_unused]] auto e = val.end();
    }

    // Const.
    {
      SplitViewCopyable v(input, "b");
      const auto val = *v.begin();

      static_assert(std::same_as<decltype(val.end()), std::default_sentinel_t>);
      static_assert(noexcept(val.end()));
      [[maybe_unused]] auto e = val.end();
    }
  }

  // `View` is an input range.
  {
    InputView input("a");

    // Non-const.
    {
      SplitViewInput v(input, 'b');
      auto val = *v.begin();

      static_assert(std::same_as<decltype(val.end()), std::default_sentinel_t>);
      static_assert(noexcept(val.end()));
      [[maybe_unused]] auto e = val.end();
    }

    // Const.
    {
      SplitViewInput v(input, 'b');
      const auto val = *v.begin();

      static_assert(std::same_as<decltype(val.end()), std::default_sentinel_t>);
      static_assert(noexcept(val.end()));
      [[maybe_unused]] auto e = val.end();
    }
  }

  return true;
}
} using namespace libcis_ns_ranges_range_adaptors_range_lazy_split_range_lazy_split_outer_value_end; // libcis


void test_ranges_range_adaptors_range_lazy_split_range_lazy_split_outer_value_end() {
  assert(test());
  static_assert(test());

  return;
}
