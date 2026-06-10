// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_drop_ctor_view).
// main -> test_ranges_range_adaptors_range_drop_ctor_view; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr drop_view(V base, range_difference_t<V> count);  // explicit since C++23

#include <cassert>
#include <ranges>

#include "test_convertible.h"
#include "test_macros.h"
#include "types.h"

// SFINAE tests.

#if TEST_STD_VER >= 23

namespace libcis_ns_ranges_range_adaptors_range_drop_ctor_view { // libcis: isolate file-scope helpers
static_assert(!test_convertible<std::ranges::drop_view<View>, View, std::ranges::range_difference_t<View>>(),
              "This constructor must be explicit");

#else

static_assert(test_convertible<std::ranges::drop_view<View>, View, std::ranges::range_difference_t<View>>(),
              "This constructor must not be explicit");

#endif // TEST_STD_VER >= 23

constexpr bool test() {
  std::ranges::drop_view dropView1(MoveOnlyView(), 4);
  assert(dropView1.size() == 4);
  assert(dropView1.begin() == globalBuff + 4);

  std::ranges::drop_view dropView2(ForwardView(), 4);
  assert(base(dropView2.begin()) == globalBuff + 4);

  return true;
}
} using namespace libcis_ns_ranges_range_adaptors_range_drop_ctor_view; // libcis


void test_ranges_range_adaptors_range_drop_ctor_view() {
  test();
  static_assert(test());

  return;
}
