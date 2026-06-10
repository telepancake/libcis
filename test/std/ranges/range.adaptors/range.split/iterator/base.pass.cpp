// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_split_iterator_base).
// main -> test_ranges_range_adaptors_range_split_iterator_base; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr iterator_t<V> base() const;

#include <cassert>
#include <ranges>

#include "../types.h"

namespace libcis_ns_ranges_range_adaptors_range_split_iterator_base { // libcis: isolate file-scope helpers
struct Iter : ForwardIterBase<Iter> {
  int i;
  constexpr Iter() = default;
  constexpr Iter(int ii) : i(ii) {}
};

constexpr bool test() {
  // base only has one const overload
  using SplitView = std::ranges::split_view<std::ranges::subrange<Iter>, std::ranges::subrange<Iter>>;
  using SplitIter = std::ranges::iterator_t<SplitView>;

  // const &
  {
    SplitView sv;
    const SplitIter it{sv, Iter{5}, {}};
    std::same_as<Iter> decltype(auto) base = it.base();
    assert(base.i == 5);
  }

  // &
  {
    SplitView sv;
    SplitIter it{sv, Iter{5}, {}};
    std::same_as<Iter> decltype(auto) base = it.base();
    assert(base.i == 5);
  }

  // &&
  {
    SplitView sv;
    SplitIter it{sv, Iter{5}, {}};
    std::same_as<Iter> decltype(auto) base = std::move(it).base();
    assert(base.i == 5);
  }

  // const &&
  {
    SplitView sv;
    const SplitIter it{sv, Iter{5}, {}};
    std::same_as<Iter> decltype(auto) base = std::move(it).base();
    assert(base.i == 5);
  }

  return true;
}
} using namespace libcis_ns_ranges_range_adaptors_range_split_iterator_base; // libcis


void test_ranges_range_adaptors_range_split_iterator_base() {
  test();
  static_assert(test());

  return;
}
