// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_transform_base).
// main -> test_ranges_range_adaptors_range_transform_base; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr V base() const& requires copy_constructible<V>
// constexpr V base() &&

#include <cassert>
#include <ranges>

#include "test_macros.h"
#include "types.h"

namespace libcis_ns_ranges_range_adaptors_range_transform_base { // libcis: isolate file-scope helpers
constexpr bool test() {
  {
    std::ranges::transform_view<MoveOnlyView, PlusOne> transformView;
    MoveOnlyView base = std::move(transformView).base();
    ASSERT_SAME_TYPE(MoveOnlyView, decltype(std::move(transformView).base()));
    assert(std::ranges::begin(base) == globalBuff);
  }

  {
    std::ranges::transform_view<CopyableView, PlusOne> transformView;
    CopyableView base1 = transformView.base();
    ASSERT_SAME_TYPE(CopyableView, decltype(transformView.base()));
    assert(std::ranges::begin(base1) == globalBuff);

    CopyableView base2 = std::move(transformView).base();
    ASSERT_SAME_TYPE(CopyableView, decltype(std::move(transformView).base()));
    assert(std::ranges::begin(base2) == globalBuff);
  }

  {
    const std::ranges::transform_view<CopyableView, PlusOne> transformView;
    const CopyableView base1 = transformView.base();
    ASSERT_SAME_TYPE(CopyableView, decltype(transformView.base()));
    assert(std::ranges::begin(base1) == globalBuff);

    const CopyableView base2 = std::move(transformView).base();
    ASSERT_SAME_TYPE(CopyableView, decltype(std::move(transformView).base()));
    assert(std::ranges::begin(base2) == globalBuff);
  }

  return true;
}
} using namespace libcis_ns_ranges_range_adaptors_range_transform_base; // libcis


void test_ranges_range_adaptors_range_transform_base() {
  test();
  static_assert(test());

  return;
}
