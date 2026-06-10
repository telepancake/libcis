// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_factories_range_repeat_view_ctor_default).
// main -> test_ranges_range_factories_range_repeat_view_ctor_default; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// repeat_view() requires default_initializable<T> = default;

#include <ranges>
#include <cassert>
#include <concepts>

namespace libcis_ns_ranges_range_factories_range_repeat_view_ctor_default { // libcis: isolate file-scope helpers
struct DefaultInt42 {
  int value = 42;
};

struct Int {
  Int(int) {}
};

static_assert(std::default_initializable<std::ranges::repeat_view<DefaultInt42>>);
static_assert(!std::default_initializable<std::ranges::repeat_view<Int>>);

constexpr bool test() {
  std::ranges::repeat_view<DefaultInt42> rv;
  assert((*rv.begin()).value == 42);

  return true;
}
} using namespace libcis_ns_ranges_range_factories_range_repeat_view_ctor_default; // libcis


void test_ranges_range_factories_range_repeat_view_ctor_default() {
  test();
  static_assert(test());

  return;
}
