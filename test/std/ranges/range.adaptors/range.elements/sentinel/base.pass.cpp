// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_elements_sentinel_base).
// main -> test_ranges_range_adaptors_range_elements_sentinel_base; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr sentinel_t<Base> base() const;

#include <cassert>
#include <ranges>
#include <tuple>
#include <type_traits>
#include <utility>

namespace libcis_ns_ranges_range_adaptors_range_elements_sentinel_base { // libcis: isolate file-scope helpers
struct Sent {
  int i;

  friend constexpr bool operator==(std::tuple<int>*, const Sent&) { return true; }
};

constexpr bool test() {
  using BaseRange = std::ranges::subrange<std::tuple<int>*, Sent>;
  using EleRange  = std::ranges::elements_view<BaseRange, 0>;
  using EleSent   = std::ranges::sentinel_t<EleRange>;

  const EleSent st{Sent{5}};
  std::same_as<Sent> decltype(auto) base = st.base();
  assert(base.i == 5);

  return true;
}
} using namespace libcis_ns_ranges_range_adaptors_range_elements_sentinel_base; // libcis


void test_ranges_range_adaptors_range_elements_sentinel_base() {
  test();
  static_assert(test());
  return;
}
