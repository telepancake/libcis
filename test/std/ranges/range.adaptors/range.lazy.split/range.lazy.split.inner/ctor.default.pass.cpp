// transferred+adapted from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_lazy_split_range_lazy_split_inner_ctor_default_a943a38e).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr inner-iterator::inner-iterator() = default;

#include <ranges>

#include "../types.h"

namespace libcis_ns_ranges_range_adaptors_range_lazy_split_range_lazy_split_inner_ctor_default_a943a38e { // libcis
constexpr bool test() {
  {
    [[maybe_unused]] InnerIterForward i;
  }

  {
    [[maybe_unused]] InnerIterInput i;
  }

  return true;
}

int main(int, char**) {
  test();
  static_assert(test());

  return 0;

    return 0;
}
} // libcis_ns_ranges_range_adaptors_range_lazy_split_range_lazy_split_inner_ctor_default_a943a38e (libcis)

