// transferred+adapted from libc++ by tools/transfer.py (slug=ranges_range_utility_range_dangling_dangling_67bbb98e).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// std::ranges::dangling;

#include <ranges>

#include <concepts>
#include <type_traits>

namespace libcis_ns_ranges_range_utility_range_dangling_dangling_67bbb98e { // libcis
static_assert(std::is_empty_v<std::ranges::dangling>);

template<int> struct S { };
static_assert(std::is_nothrow_constructible_v<std::ranges::dangling>);
static_assert(std::is_nothrow_constructible_v<std::ranges::dangling, S<0>>);
static_assert(std::is_nothrow_constructible_v<std::ranges::dangling, S<0>, S<1>>);
static_assert(std::is_nothrow_constructible_v<std::ranges::dangling, S<0>, S<1>, S<2>>);

constexpr bool test_dangling() {
  [[maybe_unused]] auto a = std::ranges::dangling();
  [[maybe_unused]] auto b = std::ranges::dangling(S<0>());
  [[maybe_unused]] auto c = std::ranges::dangling(S<0>(), S<1>());
  [[maybe_unused]] auto d = std::ranges::dangling(S<0>(), S<1>(), S<2>());
  return true;
}

int main(int, char**) {
  static_assert(test_dangling());
  test_dangling();
  return 0;

    return 0;
}
} // libcis_ns_ranges_range_utility_range_dangling_dangling_67bbb98e (libcis)

