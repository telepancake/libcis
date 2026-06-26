// transferred+adapted from libc++ by tools/transfer.py (slug=ranges_range_factories_range_iota_view_iterator_decrement_24c814c5).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr iterator& operator--() requires decrementable<W>;
// constexpr iterator operator--(int) requires decrementable<W>;

#include <cassert>
#include <ranges>
#include <type_traits>

#include "test_macros.h"
#include "../types.h"

namespace libcis_ns_ranges_range_factories_range_iota_view_iterator_decrement_24c814c5 { // libcis
template<class T>
concept Decrementable =
  requires(T i) {
    --i;
  } ||
  requires(T i) {
    i--;
  };

constexpr bool test() {
  {
    std::ranges::iota_view<int> io(0);
    auto iter1 = std::next(io.begin());
    auto iter2 = std::next(io.begin());
    assert(iter1 == iter2);
    assert(--iter1 != iter2--);
    assert(iter1 == iter2);

    static_assert(!std::is_reference_v<decltype(iter2--)>);
    static_assert( std::is_reference_v<decltype(--iter2)>);
    static_assert(std::same_as<std::remove_reference_t<decltype(--iter2)>, decltype(iter2--)>);
  }
  {
    std::ranges::iota_view io(SomeInt(0));
    auto iter1 = std::next(io.begin());
    auto iter2 = std::next(io.begin());
    assert(iter1 == iter2);
    assert(--iter1 != iter2--);
    assert(iter1 == iter2);

    static_assert(!std::is_reference_v<decltype(iter2--)>);
    static_assert( std::is_reference_v<decltype(--iter2)>);
    static_assert(std::same_as<std::remove_reference_t<decltype(--iter2)>, decltype(iter2--)>);
  }

  static_assert(!Decrementable<std::ranges::iota_view<NotDecrementable>>);

  return true;
}

int main(int, char**) {
  test();
  static_assert(test());

  return 0;

    return 0;
}
} // libcis_ns_ranges_range_factories_range_iota_view_iterator_decrement_24c814c5 (libcis)

