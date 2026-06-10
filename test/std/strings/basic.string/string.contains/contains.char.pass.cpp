// AST-transferred from libc++ by tools/transfer.py (slug=strings_basic_string_string_contains_contains_char).
// main -> test_strings_basic_string_string_contains_contains_char; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <string>

// constexpr bool contains(charT x) const noexcept;

#include <string>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_strings_basic_string_string_contains_contains_char { // libcis: isolate file-scope helpers
template <class S>
constexpr void test_string() {
  S s1{};
  S s2{"abcde", 5};

  ASSERT_NOEXCEPT(s1.contains('e'));

  assert(!s1.contains('c'));
  assert(!s1.contains('e'));
  assert(!s1.contains('x'));
  assert(s2.contains('c'));
  assert(s2.contains('e'));
  assert(!s2.contains('x'));
}

constexpr bool test() {
  test_string<std::string>();

  return true;
}
} using namespace libcis_ns_strings_basic_string_string_contains_contains_char; // libcis


void test_strings_basic_string_string_contains_contains_char() {
  test();
  static_assert(test());

  return;
}
