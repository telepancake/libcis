// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_split_iterator_ctor_default).
// main -> test_ranges_range_adaptors_range_split_iterator_ctor_default; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// iterator() = default;

#include <cassert>
#include <ranges>

#include "../types.h"

namespace libcis_ns_ranges_range_adaptors_range_split_iterator_ctor_default { // libcis: isolate file-scope helpers
struct PODIter : ForwardIterBase<PODIter> {
  int i;
};

constexpr bool test() {
  using SplitView = std::ranges::split_view<std::ranges::subrange<PODIter>, std::ranges::subrange<PODIter>>;
  using SplitIter = std::ranges::iterator_t<SplitView>;
  {
    SplitIter iter;
    assert(iter.base().i == 0); // PODIter has to be initialised to have value 0
  }

  {
    SplitIter iter = {};        // explicit(false)
    assert(iter.base().i == 0); // PODIter has to be initialised to have value 0
  }

  return true;
}
} using namespace libcis_ns_ranges_range_adaptors_range_split_iterator_ctor_default; // libcis


void test_ranges_range_adaptors_range_split_iterator_ctor_default() {
  test();
  static_assert(test());

  return;
}
