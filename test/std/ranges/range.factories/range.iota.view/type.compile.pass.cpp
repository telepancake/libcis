// transferred+adapted from libc++ by tools/transfer.py (slug=ranges_range_factories_range_iota_view_type_325a6b6f).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

#include <ranges>

// Test that we SFINAE away iota_view<bool>.

namespace libcis_ns_ranges_range_factories_range_iota_view_type_325a6b6f { // libcis
template<class T> std::ranges::iota_view<T> f(int);
template<class T> void f(...);

void test() {
  f<bool>(42);
}
} // libcis_ns_ranges_range_factories_range_iota_view_type_325a6b6f (libcis)

