// transferred+adapted from libc++ by tools/transfer.py (slug=ranges_range_factories_range_iota_view_begin_79c223b7).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr iterator begin() const;

#include <cassert>
#include <ranges>
#include <utility>

#include "test_macros.h"
#include "types.h"

namespace libcis_ns_ranges_range_factories_range_iota_view_begin_79c223b7 { // libcis
template<class T>
constexpr void testType() {
  {
    std::ranges::iota_view<T> io(T(0));
    assert(*io.begin() == T(0));
  }
  {
    std::ranges::iota_view<T> io(T(10));
    assert(*io.begin() == T(10));
    assert(*std::move(io).begin() == T(10));
  }
  {
    const std::ranges::iota_view<T> io(T(0));
    assert(*io.begin() == T(0));
  }
  {
    const std::ranges::iota_view<T> io(T(10));
    assert(*io.begin() == T(10));
  }
}

constexpr bool test() {
  testType<SomeInt>();
  testType<long long>();
  testType<unsigned long long>();
  testType<signed long>();
  testType<unsigned long>();
  testType<int>();
  testType<unsigned>();
  testType<short>();
  testType<unsigned short>();

  return true;
}

int main(int, char**) {
  test();
  static_assert(test());

  return 0;

    return 0;
}
} // libcis_ns_ranges_range_factories_range_iota_view_begin_79c223b7 (libcis)

