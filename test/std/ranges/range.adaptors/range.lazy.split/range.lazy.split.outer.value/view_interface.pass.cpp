// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_lazy_split_range_lazy_split_outer_value_view_interface).
// main -> test_ranges_range_adaptors_range_lazy_split_range_lazy_split_outer_value_view_interface; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// class std::ranges::lazy_split_view::outer-iterator::value_type;

#include <ranges>

#include <cassert>
#include <concepts>
#include "../types.h"

namespace libcis_ns_ranges_range_adaptors_range_lazy_split_range_lazy_split_outer_value_view_interface { // libcis: isolate file-scope helpers
using V = ValueTypeForward;
static_assert(std::ranges::forward_range<V>);
static_assert(std::ranges::view<V>);

static_assert(std::is_base_of_v<std::ranges::view_interface<ValueTypeForward>, ValueTypeForward>);

constexpr bool test() {
  // empty()
  {
    {
      SplitViewForward v("abc def", " ");
      auto val = *v.begin();
      assert(!val.empty());
    }

    {
      SplitViewForward v;
      auto val = *v.begin();
      assert(val.empty());
    }
  }

  // operator bool()
  {
    {
      SplitViewForward v("abc def", " ");
      auto val = *v.begin();
      assert(val);
    }

    {
      SplitViewForward v;
      auto val = *v.begin();
      assert(!val);
    }
  }

  // front()
  {
    SplitViewForward v("abc def", " ");
    auto val = *v.begin();
    assert(val.front() == 'a');
  }

  return true;
}
} using namespace libcis_ns_ranges_range_adaptors_range_lazy_split_range_lazy_split_outer_value_view_interface; // libcis


void test_ranges_range_adaptors_range_lazy_split_range_lazy_split_outer_value_view_interface() {
  test();
  static_assert(test());

  return;
}
