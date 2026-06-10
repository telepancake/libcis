// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_factories_range_single_view_begin).
// main -> test_ranges_range_factories_range_single_view_begin; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr T* begin() noexcept;
// constexpr const T* begin() const noexcept;

#include <ranges>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_ranges_range_factories_range_single_view_begin { // libcis: isolate file-scope helpers
struct Empty {};
struct BigType { char buffer[64] = {10}; };

constexpr bool test() {
  {
    auto sv = std::ranges::single_view<int>(42);
    assert(*sv.begin() == 42);

    ASSERT_SAME_TYPE(decltype(sv.begin()), int*);
    static_assert(noexcept(sv.begin()));
  }
  {
    const auto sv = std::ranges::single_view<int>(42);
    assert(*sv.begin() == 42);

    ASSERT_SAME_TYPE(decltype(sv.begin()), const int*);
    static_assert(noexcept(sv.begin()));
  }

  {
    auto sv = std::ranges::single_view<Empty>(Empty());
    assert(sv.begin() != nullptr);

    ASSERT_SAME_TYPE(decltype(sv.begin()), Empty*);
  }
  {
    const auto sv = std::ranges::single_view<Empty>(Empty());
    assert(sv.begin() != nullptr);

    ASSERT_SAME_TYPE(decltype(sv.begin()), const Empty*);
  }

  {
    auto sv = std::ranges::single_view<BigType>(BigType());
    assert(sv.begin()->buffer[0] == 10);

    ASSERT_SAME_TYPE(decltype(sv.begin()), BigType*);
  }
  {
    const auto sv = std::ranges::single_view<BigType>(BigType());
    assert(sv.begin()->buffer[0] == 10);

    ASSERT_SAME_TYPE(decltype(sv.begin()), const BigType*);
  }

  return true;
}
} using namespace libcis_ns_ranges_range_factories_range_single_view_begin; // libcis


void test_ranges_range_factories_range_single_view_begin() {
  test();
  static_assert(test());

  return;
}
