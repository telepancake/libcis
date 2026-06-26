// transferred+adapted from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_lazy_split_range_lazy_split_outer_value_ctor_default_b35f262c).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// std::ranges::lazy_split_view::outer-iterator::value_type::value_type()

#include <ranges>

#include "../types.h"

namespace libcis_ns_ranges_range_adaptors_range_lazy_split_range_lazy_split_outer_value_ctor_default_b35f262c { // libcis
constexpr bool test() {
  {
    [[maybe_unused]] ValueTypeForward val;
  }

  {
    [[maybe_unused]] ValueTypeForward val = {};
  }

  {
    [[maybe_unused]] ValueTypeInput val;
  }

  {
    [[maybe_unused]] ValueTypeInput val = {};
  }

  return true;
}

int main(int, char**) {
  test();
  static_assert(test());

  return 0;

    return 0;
}
} // libcis_ns_ranges_range_adaptors_range_lazy_split_range_lazy_split_outer_value_ctor_default_b35f262c (libcis)

