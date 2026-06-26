// transferred+adapted from libc++ by tools/transfer.py (slug=ranges_range_factories_range_iota_view_iterator_ctor_default_d36b4148).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// iterator() requires default_initializable<W> = default;

#include <ranges>
#include <cassert>

#include "test_macros.h"
#include "../types.h"

namespace libcis_ns_ranges_range_factories_range_iota_view_iterator_ctor_default_d36b4148 { // libcis
constexpr bool test() {
  using Iter = std::ranges::iterator_t<std::ranges::iota_view<Int42<DefaultTo42>>>;
  Iter iter;
  assert((*iter).value_ == 42);

  return true;
}

int main(int, char**) {
  test();
  static_assert(test());

  return 0;

    return 0;
}
} // libcis_ns_ranges_range_factories_range_iota_view_iterator_ctor_default_d36b4148 (libcis)

