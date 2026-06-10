// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_factories_range_single_view_ctor_value).
// main -> test_ranges_range_factories_range_single_view_ctor_value; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr explicit single_view(const T& t);
// constexpr explicit single_view(T&& t);

#include <cassert>
#include <ranges>
#include <utility>

#include "test_macros.h"

namespace libcis_ns_ranges_range_factories_range_single_view_ctor_value { // libcis: isolate file-scope helpers
struct Empty {};
struct BigType { char buffer[64] = {10}; };

constexpr bool test() {
  {
    BigType bt;
    std::ranges::single_view<BigType> sv(bt);
    assert(sv.data()->buffer[0] == 10);
    assert(sv.size() == 1);
  }
  {
    const BigType bt;
    const std::ranges::single_view<BigType> sv(bt);
    assert(sv.data()->buffer[0] == 10);
    assert(sv.size() == 1);
  }

  {
    BigType bt;
    std::ranges::single_view<BigType> sv(std::move(bt));
    assert(sv.data()->buffer[0] == 10);
    assert(sv.size() == 1);
  }
  {
    const BigType bt;
    const std::ranges::single_view<BigType> sv(std::move(bt));
    assert(sv.data()->buffer[0] == 10);
    assert(sv.size() == 1);
  }

  return true;
}
} using namespace libcis_ns_ranges_range_factories_range_single_view_ctor_value; // libcis


void test_ranges_range_factories_range_single_view_ctor_value() {
  test();
  static_assert(test());

  return;
}
