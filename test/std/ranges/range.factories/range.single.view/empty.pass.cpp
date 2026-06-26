// transferred+adapted from libc++ by tools/transfer.py (slug=ranges_range_factories_range_single_view_empty_b5baaa88).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// static constexpr bool empty() noexcept;

#include <cassert>
#include <concepts>
#include <ranges>
#include <utility>

#include "test_macros.h"

namespace libcis_ns_ranges_range_factories_range_single_view_empty_b5baaa88 { // libcis
struct Empty {};
struct BigType {
  char buffer[64] = {10};
};

template <typename T>
constexpr void test_empty(T value) {
  using SingleView = std::ranges::single_view<T>;

  {
    std::same_as<bool> decltype(auto) result = SingleView::empty();
    assert(result == false);
    static_assert(noexcept(SingleView::empty()));
  }

  {
    SingleView sv{value};

    std::same_as<bool> decltype(auto) result = std::ranges::empty(sv);
    assert(result == false);
    static_assert(noexcept(std::ranges::empty(sv)));
  }
  {
    const SingleView sv{value};

    std::same_as<bool> decltype(auto) result = std::ranges::empty(sv);
    assert(result == false);
    static_assert(noexcept(std::ranges::empty(std::as_const(sv))));
  }
}

constexpr bool test() {
  test_empty<int>(92);
  test_empty<Empty>(Empty{});
  test_empty<BigType>(BigType{});

  return true;
}

int main(int, char**) {
  test();
  static_assert(test());

  return 0;

    return 0;
}
} // libcis_ns_ranges_range_factories_range_single_view_empty_b5baaa88 (libcis)

