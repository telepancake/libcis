// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_zip_sentinel_ctor_default).
// main -> test_ranges_range_adaptors_range_zip_sentinel_ctor_default; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// sentinel() = default;

#include <cassert>
#include <ranges>
#include <tuple>

namespace libcis_ns_ranges_range_adaptors_range_zip_sentinel_ctor_default { // libcis: isolate file-scope helpers
struct PODSentinel {
  bool b; // deliberately uninitialised

  friend constexpr bool operator==(int*, const PODSentinel& s) { return s.b; }
};

struct Range : std::ranges::view_base {
  int* begin() const;
  PODSentinel end();
};

constexpr bool test() {
  {
    using R = std::ranges::zip_view<Range>;
    using Sentinel = std::ranges::sentinel_t<R>;
    static_assert(!std::is_same_v<Sentinel, std::ranges::iterator_t<R>>);

    std::ranges::iterator_t<R> it;

    Sentinel s1;
    assert(it != s1); // PODSentinel.b is initialised to false

    Sentinel s2 = {};
    assert(it != s2); // PODSentinel.b is initialised to false
  }
  return true;
}
} using namespace libcis_ns_ranges_range_adaptors_range_zip_sentinel_ctor_default; // libcis


void test_ranges_range_adaptors_range_zip_sentinel_ctor_default() {
  test();
  static_assert(test());

  return;
}
