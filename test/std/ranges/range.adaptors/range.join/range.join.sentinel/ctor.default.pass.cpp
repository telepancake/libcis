// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_join_range_join_sentinel_ctor_default).
// main -> test_ranges_range_adaptors_range_join_range_join_sentinel_ctor_default; file-scope helpers isolated in anon namespace.
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

#include "test_macros.h"
#include "../types.h"

namespace libcis_ns_ranges_range_adaptors_range_join_range_join_sentinel_ctor_default { // libcis: isolate file-scope helpers
constexpr bool test() {
  std::ranges::sentinel_t<std::ranges::join_view<CopyableParent>> sent;
  (void) sent;

  return true;
}
} using namespace libcis_ns_ranges_range_adaptors_range_join_range_join_sentinel_ctor_default; // libcis


void test_ranges_range_adaptors_range_join_range_join_sentinel_ctor_default() {
  test();
  static_assert(test());

  return;
}
