// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_factories_range_single_view_data).
// main -> test_ranges_range_factories_range_single_view_data; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr T* data() noexcept;
// constexpr const T* data() const noexcept;

#include <ranges>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_ranges_range_factories_range_single_view_data { // libcis: isolate file-scope helpers
struct Empty {};
struct BigType { char buffer[64] = {10}; };

constexpr bool test() {
  {
    auto sv = std::ranges::single_view<int>(42);
    assert(*sv.data() == 42);

    ASSERT_SAME_TYPE(decltype(sv.data()), int*);
    static_assert(noexcept(sv.data()));
  }
  {
    const auto sv = std::ranges::single_view<int>(42);
    assert(*sv.data() == 42);

    ASSERT_SAME_TYPE(decltype(sv.data()), const int*);
    static_assert(noexcept(sv.data()));
  }

  {
    auto sv = std::ranges::single_view<Empty>(Empty());
    assert(sv.data() != nullptr);

    ASSERT_SAME_TYPE(decltype(sv.data()), Empty*);
  }
  {
    const auto sv = std::ranges::single_view<Empty>(Empty());
    assert(sv.data() != nullptr);

    ASSERT_SAME_TYPE(decltype(sv.data()), const Empty*);
  }

  {
    auto sv = std::ranges::single_view<BigType>(BigType());
    assert(sv.data()->buffer[0] == 10);

    ASSERT_SAME_TYPE(decltype(sv.data()), BigType*);
  }
  {
    const auto sv = std::ranges::single_view<BigType>(BigType());
    assert(sv.data()->buffer[0] == 10);

    ASSERT_SAME_TYPE(decltype(sv.data()), const BigType*);
  }

  return true;
}
} using namespace libcis_ns_ranges_range_factories_range_single_view_data; // libcis


void test_ranges_range_factories_range_single_view_data() {
  test();
  static_assert(test());

  return;
}
