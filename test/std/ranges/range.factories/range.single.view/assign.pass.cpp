// transferred+adapted from libc++ by tools/transfer.py (slug=ranges_range_factories_range_single_view_assign_2ea47bc7).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// Tests that <value_> is a <copyable-box>.

#include <cassert>
#include <ranges>
#include <utility>

#include "test_macros.h"

namespace libcis_ns_ranges_range_factories_range_single_view_assign_2ea47bc7 { // libcis
struct NotAssignable {
  NotAssignable() = default;
  NotAssignable(const NotAssignable&) = default;
  NotAssignable(NotAssignable&&) = default;

  NotAssignable& operator=(const NotAssignable&) = delete;
  NotAssignable& operator=(NotAssignable&&) = delete;
};

constexpr bool test() {
  const std::ranges::single_view<NotAssignable> a;
  std::ranges::single_view<NotAssignable> b;
  b = a;
  b = std::move(a);

  return true;
}

int main(int, char**) {
  test();
  static_assert(test());

  return 0;

    return 0;
}
} // libcis_ns_ranges_range_factories_range_single_view_assign_2ea47bc7 (libcis)

