// transferred+adapted from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_join_range_join_iterator_ctor_default_15b68f92).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// iterator() = default;

#include <ranges>

#include <cassert>
#include <type_traits>
#include "test_iterators.h"
#include "../types.h"

namespace libcis_ns_ranges_range_adaptors_range_join_range_join_iterator_ctor_default_15b68f92 { // libcis
template <class It>
struct view : std::ranges::view_base {
  It begin() const;
  sentinel_wrapper<It> end() const;
};

template <class It>
constexpr void test_default_constructible() {
  using JoinView = std::ranges::join_view<view<It>>;
  using JoinIterator = std::ranges::iterator_t<JoinView>;
  static_assert(std::is_default_constructible_v<JoinIterator>);
  [[maybe_unused]] JoinIterator it;
}

constexpr bool test() {
  test_default_constructible<cpp17_input_iterator<ChildView*>>();
  test_default_constructible<cpp20_input_iterator<ChildView*>>();
  test_default_constructible<forward_iterator<ChildView*>>();
  test_default_constructible<bidirectional_iterator<ChildView*>>();
  test_default_constructible<random_access_iterator<ChildView*>>();
  test_default_constructible<contiguous_iterator<ChildView*>>();
  return true;
}

int main(int, char**) {
  test();
  static_assert(test());
  return 0;

    return 0;
}
} // libcis_ns_ranges_range_adaptors_range_join_range_join_iterator_ctor_default_15b68f92 (libcis)

