// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_common_view_ctor_default).
// main -> test_ranges_range_adaptors_range_common_view_ctor_default; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// common_view() requires default_initializable<V> = default;

#include <ranges>

#include <cassert>
#include <concepts>

#include "test_iterators.h"
#include "types.h"

void test_ranges_range_adaptors_range_common_view_ctor_default() {
  static_assert(!std::default_initializable<std::ranges::common_view<MoveOnlyView>>);
  static_assert( std::default_initializable<std::ranges::common_view<DefaultConstructibleView>>);

  std::ranges::common_view<DefaultConstructibleView> common;
  assert(common.begin() == nullptr);
  assert(common.end() == nullptr);

  return;
}
