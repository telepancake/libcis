// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_elements_ctor_view).
// main -> test_ranges_range_adaptors_range_elements_ctor_view; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr explicit elements_view(V base);

#include <cassert>
#include <ranges>
#include <tuple>
#include <type_traits>
#include <utility>

#include "MoveOnly.h"

namespace libcis_ns_ranges_range_adaptors_range_elements_ctor_view { // libcis: isolate file-scope helpers
struct View : std::ranges::view_base {
  MoveOnly mo;
  std::tuple<int>* begin() const;
  std::tuple<int>* end() const;
};

// Test Explicit
static_assert(std::is_constructible_v<std::ranges::elements_view<View, 0>, View>);
static_assert(!std::is_convertible_v<View, std::ranges::elements_view<View, 0>>);

constexpr bool test() {
  {
    std::ranges::elements_view<View, 0> ev{View{{}, MoveOnly{5}}};
    assert(std::move(ev).base().mo.get() == 5);
  }

  return true;
}
} using namespace libcis_ns_ranges_range_adaptors_range_elements_ctor_view; // libcis


void test_ranges_range_adaptors_range_elements_ctor_view() {
  test();
  static_assert(test());
  return;
}
