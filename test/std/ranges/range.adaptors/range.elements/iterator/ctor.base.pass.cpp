// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_elements_iterator_ctor_base).
// main -> test_ranges_range_adaptors_range_elements_iterator_ctor_base; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr explicit iterator(iterator_t<Base> current);

#include <cassert>
#include <ranges>
#include <tuple>

#include "../types.h"

// Test explicit
namespace libcis_ns_ranges_range_adaptors_range_elements_iterator_ctor_base { // libcis: isolate file-scope helpers
using BaseIter     = std::tuple<int>*;
using ElementsIter = std::ranges::iterator_t<std::ranges::elements_view<std::ranges::subrange<BaseIter, BaseIter>, 0>>;

static_assert(std::is_constructible_v<ElementsIter, BaseIter>);
static_assert(!std::is_convertible_v<BaseIter, ElementsIter>);

struct TracedMoveIter : IterBase<TracedMoveIter>{
  bool moved = false;

  constexpr TracedMoveIter()                      = default;
  constexpr TracedMoveIter(const TracedMoveIter&) = default;
  constexpr TracedMoveIter(TracedMoveIter&&) : moved{true} {}
  constexpr TracedMoveIter& operator=(TracedMoveIter&&)      = default;
  constexpr TracedMoveIter& operator=(const TracedMoveIter&) = default;
};

struct TracedMoveView : std::ranges::view_base {
  TracedMoveIter begin() const;
  TracedMoveIter end() const;
};

constexpr bool test() {
  using Iter = std::ranges::iterator_t<std::ranges::elements_view<TracedMoveView, 0>>;
  Iter iter{TracedMoveIter{}};
  assert(iter.base().moved);

  return true;
}
} using namespace libcis_ns_ranges_range_adaptors_range_elements_iterator_ctor_base; // libcis


void test_ranges_range_adaptors_range_elements_iterator_ctor_base() {
  test();
  static_assert(test());

  return;
}
