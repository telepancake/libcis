// transferred+adapted from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_take_while_general_becc760f).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// Some basic examples of how take_while_view might be used in the wild. This is a general
// collection of sample algorithms and functions that try to mock general usage of
// this view.

#include <algorithm>
#include <cassert>
#include <ranges>

namespace libcis_ns_ranges_range_adaptors_range_take_while_general_becc760f { // libcis
int main(int, char**) {
  {
    auto input      = {0, 1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1, 0};
    auto small      = [](const auto x) noexcept { return x < 5; };
    auto small_ints = input | std::views::take_while(small);
    auto expected   = {0, 1, 2, 3, 4};
    assert(std::ranges::equal(small_ints, expected));
  }
  return 0;

    return 0;
}
} // libcis_ns_ranges_range_adaptors_range_take_while_general_becc760f (libcis)

