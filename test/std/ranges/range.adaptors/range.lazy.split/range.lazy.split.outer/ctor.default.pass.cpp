// transferred+adapted from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_lazy_split_range_lazy_split_outer_ctor_default_1218795a).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// std::ranges::lazy_split_view::outer-iterator::outer-iterator()

#include <ranges>

#include "../types.h"

namespace libcis_ns_ranges_range_adaptors_range_lazy_split_range_lazy_split_outer_ctor_default_1218795a { // libcis
constexpr bool test() {
  // `View` is a forward range.
  {
    [[maybe_unused]] OuterIterForward i;
  }

  {
    [[maybe_unused]] OuterIterForward i = {};
  }

  // `View` is an input range.
  {
    [[maybe_unused]] OuterIterInput i;
  }

  {
    [[maybe_unused]] OuterIterInput i = {};
  }

  return true;
}

int main(int, char**) {
  test();
  static_assert(test());

  return 0;

    return 0;
}
} // libcis_ns_ranges_range_adaptors_range_lazy_split_range_lazy_split_outer_ctor_default_1218795a (libcis)

