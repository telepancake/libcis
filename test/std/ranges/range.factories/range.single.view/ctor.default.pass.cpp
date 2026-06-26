// transferred+adapted from libc++ by tools/transfer.py (slug=ranges_range_factories_range_single_view_ctor_default_2b02e486).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// single_view() requires default_initializable<T> = default;

#include <ranges>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_ranges_range_factories_range_single_view_ctor_default_2b02e486 { // libcis
struct BigType { char buffer[64] = {10}; };

template<bool DefaultCtorEnabled>
struct IsDefaultConstructible {
  IsDefaultConstructible() requires DefaultCtorEnabled = default;
};

constexpr bool test() {
  static_assert( std::default_initializable<std::ranges::single_view<IsDefaultConstructible<true>>>);
  static_assert(!std::default_initializable<std::ranges::single_view<IsDefaultConstructible<false>>>);

  {
    std::ranges::single_view<BigType> sv;
    assert(sv.data()->buffer[0] == 10);
    assert(sv.size() == 1);
  }
  {
    const std::ranges::single_view<BigType> sv;
    assert(sv.data()->buffer[0] == 10);
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
} // libcis_ns_ranges_range_factories_range_single_view_ctor_default_2b02e486 (libcis)

