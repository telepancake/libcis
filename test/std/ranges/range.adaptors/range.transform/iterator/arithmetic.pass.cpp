// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_transform_iterator_arithmetic).
// main -> test_ranges_range_adaptors_range_transform_iterator_arithmetic; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// transform_view::<iterator>::operator{++,--,+=,-=}

#include <cassert>
#include <ranges>

#include "test_macros.h"
#include "../types.h"

namespace libcis_ns_ranges_range_adaptors_range_transform_iterator_arithmetic { // libcis: isolate file-scope helpers
constexpr bool test() {
  std::ranges::transform_view<MoveOnlyView, PlusOne> transformView;
  auto iter = std::move(transformView).begin();
  assert((++iter).base() == globalBuff + 1);

  assert((iter++).base() == globalBuff + 1);
  assert(iter.base() == globalBuff + 2);

  assert((--iter).base() == globalBuff + 1);
  assert((iter--).base() == globalBuff + 1);
  assert(iter.base() == globalBuff);

  // Check that decltype(InputIter++) == void.
  ASSERT_SAME_TYPE(decltype(
    std::declval<std::ranges::iterator_t<std::ranges::transform_view<InputView, PlusOne>>>()++),
    void);

  assert((iter += 4).base() == globalBuff + 4);
  assert((iter -= 3).base() == globalBuff + 1);

  return true;
}
} using namespace libcis_ns_ranges_range_adaptors_range_transform_iterator_arithmetic; // libcis


void test_ranges_range_adaptors_range_transform_iterator_arithmetic() {
  test();
  static_assert(test());

  return;
}
