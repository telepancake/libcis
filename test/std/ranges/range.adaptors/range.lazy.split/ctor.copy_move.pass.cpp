// transferred+adapted from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_lazy_split_ctor_copy_move_4bd8c5ee).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// Test the implicitly-generated copy and move constructors since `lazy_split_view` has non-trivial members.

#include <ranges>

#include <cassert>
#include <string_view>
#include <utility>
#include "types.h"

namespace libcis_ns_ranges_range_adaptors_range_lazy_split_ctor_copy_move_4bd8c5ee { // libcis
constexpr bool test() {
  // Can copy `lazy_split_view`.
  {
    // Forward range.
    {
      std::ranges::lazy_split_view<std::string_view, std::string_view> v1("abc def", " ");
      auto v2 = v1;
      assert(v2.base() == v1.base());
    }

    // Input range.
    {
      SplitViewInput v1("abc def", ' ');
      auto v2 = v1;
      assert(v2.base() == v1.base());
    }
  }

  // Can move `lazy_split_view`.
  {
    // Forward range.
    {
      std::string_view base = "abc def";
      std::ranges::lazy_split_view<std::string_view, std::string_view> v1(base, " ");
      auto v2 = std::move(v1);
      assert(v2.base() == base);
    }

    // Input range.
    {
      InputView base("abc def");
      SplitViewInput v1(base, ' ');
      auto v2 = std::move(v1);
      assert(v2.base() == base);
    }
  }

  // `non-propagating-cache` is not copied.
  {
    SplitViewInput v1("abc def ghi", ' ');
    auto outer_iter1 = v1.begin();
    ++outer_iter1;
    auto val1 = *outer_iter1;
    auto i1 = val1.begin();
    assert(*i1 == 'd');
    ++i1;
    assert(*i1 == 'e');

    auto v2 = v1;
    auto val2 = *v2.begin();
    auto i2 = val2.begin();
    assert(*i2 == 'a');
    ++i2;
    assert(*i2 == 'b');
  }

  return true;
}

int main(int, char**) {
  test();
  static_assert(test());

  return 0;

    return 0;
}
} // libcis_ns_ranges_range_adaptors_range_lazy_split_ctor_copy_move_4bd8c5ee (libcis)

