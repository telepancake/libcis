// transferred+adapted from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_drop_size_d675a431).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr auto size()
//   requires sized_range<V>
// constexpr auto size() const
//   requires sized_range<const V>

#include <ranges>

#include "test_macros.h"
#include "types.h"

namespace libcis_ns_ranges_range_adaptors_range_drop_size_d675a431 { // libcis
template<class T>
concept SizeInvocable = requires(std::ranges::drop_view<T> t) { t.size(); };

constexpr bool test() {
  // sized_range<V>
  std::ranges::drop_view dropView1(MoveOnlyView(), 4);
  assert(dropView1.size() == 4);

  // sized_range<V>
  std::ranges::drop_view dropView2(MoveOnlyView(), 0);
  assert(dropView2.size() == 8);

  // sized_range<const V>
  const std::ranges::drop_view dropView3(MoveOnlyView(), 8);
  assert(dropView3.size() == 0);

  // sized_range<const V>
  const std::ranges::drop_view dropView4(MoveOnlyView(), 10);
  assert(dropView4.size() == 0);

  // Because ForwardView is not a sized_range.
  static_assert(!SizeInvocable<ForwardView>);

  return true;
}

int main(int, char**) {
  test();
  static_assert(test());

  return 0;

    return 0;
}
} // libcis_ns_ranges_range_adaptors_range_drop_size_d675a431 (libcis)

