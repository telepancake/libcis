// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_take_while_sentinel_ctor_default).
// main -> test_ranges_range_adaptors_range_take_while_sentinel_ctor_default; file-scope helpers isolated in anon namespace.
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

namespace libcis_ns_ranges_range_adaptors_range_take_while_sentinel_ctor_default { // libcis: isolate file-scope helpers
struct Sent {
  bool b; // deliberately uninitialised

  friend constexpr bool operator==(int*, const Sent& s) { return s.b; }
};

struct Range : std::ranges::view_base {
  int* begin() const;
  Sent end();
};

constexpr bool test() {
  {
    using R        = std::ranges::take_while_view<Range, bool (*)(int)>;
    using Sentinel = std::ranges::sentinel_t<R>;

    Sentinel s1 = {};
    assert(!s1.base().b);
  }
  return true;
}
} using namespace libcis_ns_ranges_range_adaptors_range_take_while_sentinel_ctor_default; // libcis


void test_ranges_range_adaptors_range_take_while_sentinel_ctor_default() {
  test();
  static_assert(test());

  return;
}
