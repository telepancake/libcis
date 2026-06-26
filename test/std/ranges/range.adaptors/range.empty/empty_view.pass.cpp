// transferred+adapted from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_empty_empty_view_b2e03746).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// template<class T>
// class empty_view;

#include <ranges>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_ranges_range_adaptors_range_empty_empty_view_b2e03746 { // libcis
template<class T>
constexpr void testType() {
  static_assert(std::ranges::range<std::ranges::empty_view<T>>);
  static_assert(std::ranges::range<const std::ranges::empty_view<T>>);
  static_assert(std::ranges::view<std::ranges::empty_view<T>>);

  std::ranges::empty_view<T> empty;

  assert(empty.begin() == nullptr);
  assert(empty.end() == nullptr);
  assert(empty.data() == nullptr);
  assert(empty.size() == 0);
  assert(empty.empty() == true);

  assert(std::ranges::begin(empty) == nullptr);
  assert(std::ranges::end(empty) == nullptr);
  assert(std::ranges::data(empty) == nullptr);
  assert(std::ranges::size(empty) == 0);
  assert(std::ranges::empty(empty) == true);
}

struct Empty {};
struct BigType { char buff[8]; };

template<class T>
concept ValidEmptyView = requires { typename std::ranges::empty_view<T>; };

constexpr bool test() {
  // Not objects:
  static_assert(!ValidEmptyView<int&>);
  static_assert(!ValidEmptyView<void>);

  testType<int>();
  testType<const int>();
  testType<int*>();
  testType<Empty>();
  testType<const Empty>();
  testType<BigType>();

  return true;
}

int main(int, char**) {
  test();
  static_assert(test());

  return 0;

    return 0;
}
} // libcis_ns_ranges_range_adaptors_range_empty_empty_view_b2e03746 (libcis)

