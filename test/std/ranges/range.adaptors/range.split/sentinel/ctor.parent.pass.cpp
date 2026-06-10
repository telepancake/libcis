// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_split_sentinel_ctor_parent).
// main -> test_ranges_range_adaptors_range_split_sentinel_ctor_parent; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr explicit sentinel(split_view& parent);

#include <cassert>
#include <ranges>
#include <type_traits>

#include "test_iterators.h"

// test explicit
namespace libcis_ns_ranges_range_adaptors_range_split_sentinel_ctor_parent { // libcis: isolate file-scope helpers
using Range     = std::ranges::subrange<int*, sentinel_wrapper<int*>>;
using SplitView = std::ranges::split_view<Range, std::ranges::single_view<int>>;
using SplitSent = std::ranges::sentinel_t<SplitView>;

static_assert(std::is_constructible_v<SplitSent, SplitView&>);
static_assert(!std::is_convertible_v<SplitView&, SplitSent>);

constexpr bool test() {
  {
    int buffer[] = {0, 1, 2};
    Range input{buffer, sentinel_wrapper<int*>(buffer + 3)};
    SplitView sv(input, -1);
    auto it = sv.begin();

    SplitSent sent(sv);
    assert(sent != it);

    ++it;
    assert(sent == it);
  }

  return true;
}
} using namespace libcis_ns_ranges_range_adaptors_range_split_sentinel_ctor_parent; // libcis


void test_ranges_range_adaptors_range_split_sentinel_ctor_parent() {
  test();
  static_assert(test());

  return;
}
