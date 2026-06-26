// transferred+adapted from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_lazy_split_range_lazy_split_outer_value_ctor_iter_fc30a67c).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// explicit outer-iterator::value_type::value_type(outer-iterator i)

#include <ranges>

#include <cassert>
#include "../types.h"

// Verify that the constructor is `explicit`.
namespace libcis_ns_ranges_range_adaptors_range_lazy_split_range_lazy_split_outer_value_ctor_iter_fc30a67c { // libcis
static_assert(!std::is_convertible_v<OuterIterForward, ValueTypeForward>);
static_assert(!std::is_convertible_v<OuterIterInput, ValueTypeInput>);

constexpr bool test() {
  // `View` is a forward range.
  {
    CopyableView input = "a";
    SplitViewCopyable v(input, "b");
    ValueTypeCopyable val(v.begin());
    assert(val.begin().base() == input.begin());
  }

  // `View` is an input range.
  {
    InputView input = "a";
    SplitViewInput v(input, 'b');
    ValueTypeInput val(v.begin());
    assert(*val.begin().base() == *input.begin());
  }

  return true;
}

int main(int, char**) {
  test();
  static_assert(test());

  return 0;

    return 0;
}
} // libcis_ns_ranges_range_adaptors_range_lazy_split_range_lazy_split_outer_value_ctor_iter_fc30a67c (libcis)

