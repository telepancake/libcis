// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_factories_range_repeat_view_iterator_ctor_default).
// main -> test_ranges_range_factories_range_repeat_view_iterator_ctor_default; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// iterator() = default;

#include <ranges>
#include <type_traits>

namespace libcis_ns_ranges_range_factories_range_repeat_view_iterator_ctor_default { // libcis: isolate file-scope helpers
constexpr bool test() {
  using Iter = std::ranges::iterator_t<std::ranges::repeat_view<int>>;
  static_assert(std::is_default_constructible_v<Iter>);
  [[maybe_unused]] Iter iter;

  return true;
}
} using namespace libcis_ns_ranges_range_factories_range_repeat_view_iterator_ctor_default; // libcis


void test_ranges_range_factories_range_repeat_view_iterator_ctor_default() {
  test();
  static_assert(test());

  return;
}
