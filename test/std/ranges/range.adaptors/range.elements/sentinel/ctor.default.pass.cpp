// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_elements_sentinel_ctor_default).
// main -> test_ranges_range_adaptors_range_elements_sentinel_ctor_default; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// sentinel() = default;

#include <cassert>
#include <ranges>
#include <tuple>

namespace libcis_ns_ranges_range_adaptors_range_elements_sentinel_ctor_default { // libcis: isolate file-scope helpers
struct PODSentinel {
  int i; // deliberately uninitialised

  friend constexpr bool operator==(std::tuple<int>*, const PODSentinel&) { return true; }
};

struct Range : std::ranges::view_base {
  std::tuple<int>* begin() const;
  PODSentinel end();
};

constexpr bool test() {
  using EleView  = std::ranges::elements_view<Range, 0>;
  using Sentinel = std::ranges::sentinel_t<EleView>;
  static_assert(!std::is_same_v<Sentinel, std::ranges::iterator_t<EleView>>);

  {
    Sentinel s;
    assert(s.base().i == 0);
  }
  {
    Sentinel s = {};
    assert(s.base().i == 0);
  }
  return true;
}
} using namespace libcis_ns_ranges_range_adaptors_range_elements_sentinel_ctor_default; // libcis


void test_ranges_range_adaptors_range_elements_sentinel_ctor_default() {
  test();
  static_assert(test());

  return;
}
