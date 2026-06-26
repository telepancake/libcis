// transferred+adapted from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_filter_sentinel_base).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr sentinel_t<V> base() const;

#include <ranges>

#include <array>
#include <cassert>
#include <concepts>
#include <utility>
#include "test_iterators.h"
#include "../types.h"

namespace libcis_ns_ranges_range_adaptors_range_filter_sentinel_base_7b470b8d { // libcis
template <class Iter, class Sent = sentinel_wrapper<Iter>>
constexpr void test() {
  using View = minimal_view<Iter, Sent>;
  using FilterView = std::ranges::filter_view<View, AlwaysTrue>;
  using FilterSentinel = std::ranges::sentinel_t<FilterView>;

  auto make_filter_view = [](auto begin, auto end, auto pred) {
    View view{Iter(begin), Sent(Iter(end))};
    return FilterView(std::move(view), pred);
  };

  std::array<int, 5> array{0, 1, 2, 3, 4};
  FilterView view = make_filter_view(array.data(), array.data() + array.size(), AlwaysTrue{});

  FilterSentinel const sent = view.end();
  auto result = sent.base();
  // libcis: gcc-10 constrained-placeholder defect (see tools/test_overrides);
  // type check preserved via static_assert.
  static_assert(std::is_same_v<decltype(result), Sent>);
  assert(base(base(result)) == array.data() + array.size());
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

int main(int, char**) {
  tests();
  static_assert(tests());
  return 0;
}
} // libcis_ns_ranges_range_adaptors_range_filter_sentinel_base_7b470b8d (libcis)

