// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_drop_base).
// main -> test_ranges_range_adaptors_range_drop_base; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr V base() const& requires copy_constructible<V> { return base_; }
// constexpr V base() && { return std::move(base_); }

#include <cassert>
#include <ranges>

#include "test_macros.h"
#include "types.h"

namespace libcis_ns_ranges_range_adaptors_range_drop_base { // libcis: isolate file-scope helpers
constexpr bool test() {
  std::ranges::drop_view<MoveOnlyView> dropView1;
  auto base1 = std::move(dropView1).base();
  assert(std::ranges::begin(base1) == globalBuff);

  // Note: we should *not* drop two elements here.
  std::ranges::drop_view<MoveOnlyView> dropView2(MoveOnlyView{4}, 2);
  auto base2 = std::move(dropView2).base();
  assert(std::ranges::begin(base2) == globalBuff + 4);

  std::ranges::drop_view<CopyableView> dropView3;
  auto base3 = dropView3.base();
  assert(std::ranges::begin(base3) == globalBuff);
  auto base4 = std::move(dropView3).base();
  assert(std::ranges::begin(base4) == globalBuff);

  return true;
}
} using namespace libcis_ns_ranges_range_adaptors_range_drop_base; // libcis


void test_ranges_range_adaptors_range_drop_base() {
  test();
  static_assert(test());

  return;
}
