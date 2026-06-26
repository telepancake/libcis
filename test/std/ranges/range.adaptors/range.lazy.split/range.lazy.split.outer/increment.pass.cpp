// transferred+adapted from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_lazy_split_range_lazy_split_outer_increment_e6cdcb22).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr outer-iterator& outer-iterator::operator++();
// constexpr decltype(auto) outer-iterator::operator++(int);

// Note that corner cases are tested in `range.lazy.split/general.pass.cpp`.

#include <ranges>

#include <algorithm>
#include <cassert>
#include <string>
#include "../types.h"

namespace libcis_ns_ranges_range_adaptors_range_lazy_split_range_lazy_split_outer_increment_e6cdcb22 { // libcis
constexpr bool test() {
  using namespace std::string_literals;
  // Can call `outer-iterator::operator++`; `View` is a forward range.
  {
    SplitViewForward v("abc def ghi", " ");

    // ++i
    {
      auto i = v.begin();
      assert(std::ranges::equal(*i, "abc"s));

      decltype(auto) i2 = ++i;
      static_assert(std::is_lvalue_reference_v<decltype(i2)>);
      assert(&i2 == &i);
      assert(std::ranges::equal(*i2, "def"s));
    }

    // i++
    {
      auto i = v.begin();
      assert(std::ranges::equal(*i, "abc"s));

      decltype(auto) i2 = i++;
      static_assert(!std::is_reference_v<decltype(i2)>);
      assert(std::ranges::equal(*i2, "abc"s));
      assert(std::ranges::equal(*i, "def"s));
    }
  }

  // Can call `outer-iterator::operator++`; `View` is an input range.
  {
    SplitViewInput v("abc def ghi", ' ');

    // ++i
    {
      auto i = v.begin();
      assert(std::ranges::equal(*i, "abc"s));

      decltype(auto) i2 = ++i;
      static_assert(std::is_lvalue_reference_v<decltype(i2)>);
      assert(&i2 == &i);
      assert(std::ranges::equal(*i2, "def"s));
    }

    // i++
    {
      auto i = v.begin();
      assert(std::ranges::equal(*i, "abc"s));

      static_assert(std::is_void_v<decltype(i++)>);
      i++;
      assert(std::ranges::equal(*i, "def"s));
    }
  }

  return true;
}

int main(int, char**) {
  test();
  static_assert(test());

  return 0;

    return 0;
}
} // libcis_ns_ranges_range_adaptors_range_lazy_split_range_lazy_split_outer_increment_e6cdcb22 (libcis)

