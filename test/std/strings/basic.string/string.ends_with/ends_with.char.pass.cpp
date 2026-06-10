// AST-transferred from libc++ by tools/transfer.py (slug=strings_basic_string_string_ends_with_ends_with_char).
// main -> test_strings_basic_string_string_ends_with_ends_with_char; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <string>

// constexpr bool ends_with(charT x) const noexcept;

#include <string>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_strings_basic_string_string_ends_with_ends_with_char { // libcis: isolate file-scope helpers
template <class S>
constexpr void test_string() {
  S s1{};
  S s2{"abcde", 5};

  ASSERT_NOEXCEPT(s1.ends_with('e'));

  assert(!s1.ends_with('e'));
  assert(!s1.ends_with('x'));
  assert(s2.ends_with('e'));
  assert(!s2.ends_with('x'));
}

constexpr bool test() {
  test_string<std::string>();

  return true;
}
} using namespace libcis_ns_strings_basic_string_string_ends_with_ends_with_char; // libcis


void test_strings_basic_string_string_ends_with_ends_with_char() {
  test();
  static_assert(test());

  return;
}
