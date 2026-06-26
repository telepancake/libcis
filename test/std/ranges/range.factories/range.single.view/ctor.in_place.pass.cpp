// transferred+adapted from libc++ by tools/transfer.py (slug=ranges_range_factories_range_single_view_ctor_in_place_87e2c15e).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// template<class... Args>
//   requires constructible_from<T, Args...>
// constexpr explicit single_view(in_place_t, Args&&... args);

#include <cassert>
#include <ranges>
#include <utility>

#include "test_macros.h"

namespace libcis_ns_ranges_range_factories_range_single_view_ctor_in_place_87e2c15e { // libcis
struct TakesTwoInts {
  int a_, b_;
  constexpr TakesTwoInts(int a, int b) : a_(a), b_(b) {}
};

constexpr bool test() {
  {
    std::ranges::single_view<TakesTwoInts> sv(std::in_place, 1, 2);
    assert(sv.data()->a_ == 1);
    assert(sv.data()->b_ == 2);
    assert(sv.size() == 1);
  }
  {
    const std::ranges::single_view<TakesTwoInts> sv(std::in_place, 1, 2);
    assert(sv.data()->a_ == 1);
    assert(sv.data()->b_ == 2);
    assert(sv.size() == 1);
  }

  return true;
}

int main(int, char**) {
  test();
  static_assert(test());

  return 0;

    return 0;
}
} // libcis_ns_ranges_range_factories_range_single_view_ctor_in_place_87e2c15e (libcis)

