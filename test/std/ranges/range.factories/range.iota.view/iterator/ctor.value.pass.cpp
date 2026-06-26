// transferred+adapted from libc++ by tools/transfer.py (slug=ranges_range_factories_range_iota_view_iterator_ctor_value_0c68da1d).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr explicit iterator(W value);

#include <cassert>
#include <ranges>
#include <type_traits>

#include "test_macros.h"
#include "../types.h"

namespace libcis_ns_ranges_range_factories_range_iota_view_iterator_ctor_value_0c68da1d { // libcis
constexpr bool test() {
  {
    using Iter = std::ranges::iterator_t<std::ranges::iota_view<int>>;
    auto iter = Iter(42);
    assert(*iter == 42);
  }
  {
    using Iter = std::ranges::iterator_t<std::ranges::iota_view<SomeInt>>;
    auto iter = Iter(SomeInt(42));
    assert(*iter == SomeInt(42));
  }
  {
    using Iter = std::ranges::iterator_t<std::ranges::iota_view<SomeInt>>;
    static_assert(!std::is_convertible_v<Iter, SomeInt>);
    static_assert( std::is_constructible_v<Iter, SomeInt>);
  }

  return true;
}

int main(int, char**) {
  test();
  static_assert(test());

  return 0;

    return 0;
}
} // libcis_ns_ranges_range_factories_range_iota_view_iterator_ctor_value_0c68da1d (libcis)

