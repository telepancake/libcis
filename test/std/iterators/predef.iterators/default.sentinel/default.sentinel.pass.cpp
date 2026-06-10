// AST-transferred from libc++ by tools/transfer.py (slug=iterators_predef_iterators_default_sentinel_default_sentinel).
// main -> test_iterators_predef_iterators_default_sentinel_default_sentinel; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// struct default_sentinel_t;
// inline constexpr default_sentinel_t default_sentinel;

#include <iterator>

#include <concepts>
#include <type_traits>

#include "test_macros.h"

void test_iterators_predef_iterators_default_sentinel_default_sentinel() {
  static_assert(std::is_empty_v<std::default_sentinel_t>);
  static_assert(std::semiregular<std::default_sentinel_t>);

  static_assert(std::same_as<decltype(std::default_sentinel), const std::default_sentinel_t>);

  std::default_sentinel_t s1;
  auto s2 = std::default_sentinel_t{};
  s2 = s1;

  return;
}
