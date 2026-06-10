// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_split_iterator_ctor_base).
// main -> test_ranges_range_adaptors_range_split_iterator_ctor_base; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr iterator(split_view& parent, iterator_t<V> current, subrange<iterator_t<V>> next);

#include <cassert>
#include <ranges>

#include "../types.h"

namespace libcis_ns_ranges_range_adaptors_range_split_iterator_ctor_base { // libcis: isolate file-scope helpers
struct TracedMoveIter : ForwardIterBase<TracedMoveIter> {
  bool moved = false;

  constexpr TracedMoveIter()                      = default;
  constexpr TracedMoveIter(const TracedMoveIter&) = default;
  constexpr TracedMoveIter(TracedMoveIter&&) : moved{true} {}
  constexpr TracedMoveIter& operator=(TracedMoveIter&&)      = default;
  constexpr TracedMoveIter& operator=(const TracedMoveIter&) = default;
};

struct TracedMoveView : std::ranges::view_base {
  constexpr TracedMoveIter begin() const { return {}; }
  constexpr TracedMoveIter end() const { return {}; }
};

constexpr bool test() {
  using SplitView = std::ranges::split_view<TracedMoveView, TracedMoveView>;
  using SplitIter = std::ranges::iterator_t<SplitView>;

  SplitView sv{TracedMoveView{}, TracedMoveView{}};
  SplitIter iter = {sv, sv.base().begin(), std::ranges::subrange<TracedMoveIter>{sv.base().begin(), sv.base().end()}};
  assert(iter.base().moved);

  auto subRange = *iter;
  assert(subRange.begin().moved);
  assert(subRange.end().moved);

  return true;
}
} using namespace libcis_ns_ranges_range_adaptors_range_split_iterator_ctor_base; // libcis


void test_ranges_range_adaptors_range_split_iterator_ctor_base() {
  test();
  static_assert(test());

  return;
}
