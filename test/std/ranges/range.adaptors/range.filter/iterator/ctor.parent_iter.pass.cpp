// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_filter_iterator_ctor_parent_iter).
// main -> test_ranges_range_adaptors_range_filter_iterator_ctor_parent_iter; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr std::ranges::filter_view::<iterator>(filter_view&, iterator_t<V>);

#include <ranges>

#include <array>
#include <cassert>
#include <utility>
#include "test_iterators.h"
#include "../types.h"

namespace libcis_ns_ranges_range_adaptors_range_filter_iterator_ctor_parent_iter { // libcis: isolate file-scope helpers
template <class Iter, class Sent = sentinel_wrapper<Iter>>
constexpr void test() {
  using View = minimal_view<Iter, Sent>;
  using FilterView = std::ranges::filter_view<View, AlwaysTrue>;
  using FilterIterator = std::ranges::iterator_t<FilterView>;

  std::array<int, 10> array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  View view(Iter(array.data()), Sent(Iter(array.data() + array.size())));
  Iter iter = view.begin();

  FilterView filter_view(std::move(view), AlwaysTrue{});
  FilterIterator filter_iter(filter_view, std::move(iter));
  assert(base(filter_iter.base()) == array.data());
}

constexpr bool tests() {
  test<cpp17_input_iterator<int*>>();
  test<cpp20_input_iterator<int*>>();
  test<forward_iterator<int*>>();
  test<bidirectional_iterator<int*>>();
  test<random_access_iterator<int*>>();
  test<contiguous_iterator<int*>>();
  test<int*>();
  return true;
}
} using namespace libcis_ns_ranges_range_adaptors_range_filter_iterator_ctor_parent_iter; // libcis


void test_ranges_range_adaptors_range_filter_iterator_ctor_parent_iter() {
  tests();
  static_assert(tests());
  return;
}
