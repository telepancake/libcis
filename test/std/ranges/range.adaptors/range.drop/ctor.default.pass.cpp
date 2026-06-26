// transferred+adapted from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_drop_ctor_default_61396474).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// drop_view() requires default_initializable<V> = default;

#include <ranges>

#include "test_macros.h"
#include "types.h"

namespace libcis_ns_ranges_range_adaptors_range_drop_ctor_default_61396474 { // libcis
constexpr bool test() {
  std::ranges::drop_view<MoveOnlyView> dropView1;
  assert(std::ranges::begin(dropView1) == globalBuff);

  static_assert( std::is_default_constructible_v<std::ranges::drop_view<ForwardView>>);
  static_assert(!std::is_default_constructible_v<std::ranges::drop_view<NoDefaultCtorForwardView>>);

  LIBCPP_STATIC_ASSERT( std::is_nothrow_default_constructible_v<std::ranges::drop_view<ForwardView>>);
  static_assert(!std::is_nothrow_default_constructible_v<ThrowingDefaultCtorForwardView>);
  static_assert(!std::is_nothrow_default_constructible_v<std::ranges::drop_view<ThrowingDefaultCtorForwardView>>);

  return true;
}

int main(int, char**) {
  test();
  static_assert(test());

  return 0;

    return 0;
}
} // libcis_ns_ranges_range_adaptors_range_drop_ctor_default_61396474 (libcis)

