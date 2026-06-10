// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_take_size).
// main -> test_ranges_range_adaptors_range_take_size; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr auto size() requires sized_range<V>
// constexpr auto size() const requires sized_range<const V>

#include <ranges>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "test_range.h"
#include "types.h"

namespace libcis_ns_ranges_range_adaptors_range_take_size { // libcis: isolate file-scope helpers
template<class T>
concept SizeEnabled = requires(const std::ranges::take_view<T>& tv) {
  tv.size();
};

constexpr bool test() {
  int buffer[8] = {1, 2, 3, 4, 5, 6, 7, 8};

  {
    static_assert( SizeEnabled<SizedRandomAccessView>);
    static_assert(!SizeEnabled<CopyableView>);
  }

  {
    std::ranges::take_view<SizedRandomAccessView> tv(SizedRandomAccessView{buffer}, 0);
    assert(tv.size() == 0);
  }

  {
    const std::ranges::take_view<SizedRandomAccessView> tv(SizedRandomAccessView{buffer}, 2);
    assert(tv.size() == 2);
  }

  {
    std::ranges::take_view<SizedForwardView> tv(SizedForwardView{buffer}, 4);
    assert(tv.size() == 4);
  }

  {
    const std::ranges::take_view<SizedForwardView> tv(SizedForwardView{buffer}, 6);
    assert(tv.size() == 6);
  }

  {
    std::ranges::take_view<SizedForwardView> tv(SizedForwardView{buffer}, 8);
    assert(tv.size() == 8);
  }
  {
    const std::ranges::take_view<SizedForwardView> tv(SizedForwardView{buffer}, 8);
    assert(tv.size() == 8);
  }

  {
    std::ranges::take_view<SizedForwardView> tv(SizedForwardView{buffer}, 10);
    assert(tv.size() == 8);
  }
  {
    const std::ranges::take_view<SizedForwardView> tv(SizedForwardView{buffer}, 10);
    assert(tv.size() == 8);
  }

  return true;
}
} using namespace libcis_ns_ranges_range_adaptors_range_take_size; // libcis


void test_ranges_range_adaptors_range_take_size() {
  test();
  static_assert(test());

  return;
}
