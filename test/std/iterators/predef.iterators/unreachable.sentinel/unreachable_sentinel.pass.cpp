// AST-transferred from libc++ by tools/transfer.py (slug=iterators_predef_iterators_unreachable_sentinel_unreachable_sentinel).
// main -> test_iterators_predef_iterators_unreachable_sentinel_unreachable_sentinel; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// struct unreachable_sentinel_t;
// inline constexpr unreachable_sentinel_t unreachable_sentinel;

#include <iterator>

#include <cassert>
#include <concepts>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_iterators_predef_iterators_unreachable_sentinel_unreachable_sentinel { // libcis: isolate file-scope helpers
template<class T, class U>
concept weakly_equality_comparable_with = requires(const T& t, const U& u) {
  { t == u } -> std::same_as<bool>;
  { t != u } -> std::same_as<bool>;
  { u == t } -> std::same_as<bool>;
  { u != t } -> std::same_as<bool>;
};

constexpr bool test() {
  static_assert(std::is_empty_v<std::unreachable_sentinel_t>);
  static_assert(std::semiregular<std::unreachable_sentinel_t>);

  static_assert(std::same_as<decltype(std::unreachable_sentinel), const std::unreachable_sentinel_t>);

  auto sentinel = std::unreachable_sentinel;
  int i = 42;
  assert(i != sentinel);
  assert(sentinel != i);
  assert(!(i == sentinel));
  assert(!(sentinel == i));

  assert(&i != sentinel);
  assert(sentinel != &i);
  assert(!(&i == sentinel));
  assert(!(sentinel == &i));

  int *p = nullptr;
  assert(p != sentinel);
  assert(sentinel != p);
  assert(!(p == sentinel));
  assert(!(sentinel == p));

  static_assert( weakly_equality_comparable_with<std::unreachable_sentinel_t, int>);
  static_assert( weakly_equality_comparable_with<std::unreachable_sentinel_t, int*>);
  static_assert(!weakly_equality_comparable_with<std::unreachable_sentinel_t, void*>);
  ASSERT_NOEXCEPT(sentinel == p);
  ASSERT_NOEXCEPT(sentinel != p);

  return true;
}
} using namespace libcis_ns_iterators_predef_iterators_unreachable_sentinel_unreachable_sentinel; // libcis


void test_iterators_predef_iterators_unreachable_sentinel_unreachable_sentinel() {
  test();
  static_assert(test());

  return;
}
