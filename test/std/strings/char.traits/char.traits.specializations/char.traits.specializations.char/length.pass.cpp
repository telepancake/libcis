// AST-transferred from libc++ by tools/transfer.py (slug=strings_char_traits_char_traits_specializations_char_traits_specializations_char_length).
// main -> test_strings_char_traits_char_traits_specializations_char_traits_specializations_char_length; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <string>

// template<> struct char_traits<char>

// static size_t length(const char_type* s);
// constexpr in C++17

#include <string>
#include <cassert>

#include "test_macros.h"

#if TEST_STD_VER > 14
namespace libcis_ns_strings_char_traits_char_traits_specializations_char_traits_specializations_char_length { // libcis: isolate file-scope helpers
constexpr bool test_constexpr() {
  return std::char_traits<char>::length("") == 0 && std::char_traits<char>::length("abcd") == 4;
}
} using namespace libcis_ns_strings_char_traits_char_traits_specializations_char_traits_specializations_char_length; // libcis

#endif

void test_strings_char_traits_char_traits_specializations_char_traits_specializations_char_length() {
  assert(std::char_traits<char>::length("") == 0);
  assert(std::char_traits<char>::length("a") == 1);
  assert(std::char_traits<char>::length("aa") == 2);
  assert(std::char_traits<char>::length("aaa") == 3);
  assert(std::char_traits<char>::length("aaaa") == 4);

#if TEST_STD_VER > 14
  static_assert(test_constexpr(), "");
#endif

  return;
}
