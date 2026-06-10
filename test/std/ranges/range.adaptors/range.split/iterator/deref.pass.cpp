// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_split_iterator_deref).
// main -> test_ranges_range_adaptors_range_split_iterator_deref; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr value_type operator*() const;
//   Effects: Equivalent to return {cur_, next_.begin()};

#include <cassert>
#include <ranges>

#include "../types.h"

namespace libcis_ns_ranges_range_adaptors_range_split_iterator_deref { // libcis: isolate file-scope helpers
struct Iter : ForwardIterBase<Iter> {
  int i;
  constexpr Iter() = default;
  constexpr Iter(int ii) : i(ii) {}
};

constexpr bool test() {
  using SplitView = std::ranges::split_view<std::ranges::subrange<Iter>, std::ranges::subrange<Iter>>;
  using SplitIter = std::ranges::iterator_t<SplitView>;

  {
    SplitView sv;
    Iter current{5};
    std::ranges::subrange next{Iter{6}, Iter{7}};
    const SplitIter it{sv, current, next};
    std::same_as<std::ranges::subrange<Iter>> decltype(auto) value = *it;
    assert(value.begin().i == 5);
    assert(value.end().i == 6);
  }

  return true;
}
} using namespace libcis_ns_ranges_range_adaptors_range_split_iterator_deref; // libcis


void test_ranges_range_adaptors_range_split_iterator_deref() {
  test();
  static_assert(test());

  return;
}
