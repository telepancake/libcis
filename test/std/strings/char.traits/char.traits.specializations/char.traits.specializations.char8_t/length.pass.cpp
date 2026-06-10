// AST-transferred from libc++ by tools/transfer.py (slug=strings_char_traits_char_traits_specializations_char_traits_specializations_char8_t_length).
// main -> test_strings_char_traits_char_traits_specializations_char_traits_specializations_char8_t_length; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <string>

// template<> struct char_traits<char8_t>

// static constexpr size_t length(const char_type* s);

#include <string>
#include <cassert>

#include "test_macros.h"

#ifndef TEST_HAS_NO_CHAR8_T
namespace libcis_ns_strings_char_traits_char_traits_specializations_char_traits_specializations_char8_t_length { // libcis: isolate file-scope helpers
constexpr bool test_constexpr() {
  return std::char_traits<char8_t>::length(u8"") == 0 && std::char_traits<char8_t>::length(u8"abcd") == 4;
}
} using namespace libcis_ns_strings_char_traits_char_traits_specializations_char_traits_specializations_char8_t_length; // libcis


void test_strings_char_traits_char_traits_specializations_char_traits_specializations_char8_t_length() {
  assert(std::char_traits<char8_t>::length(u8"") == 0);
  assert(std::char_traits<char8_t>::length(u8"a") == 1);
  assert(std::char_traits<char8_t>::length(u8"aa") == 2);
  assert(std::char_traits<char8_t>::length(u8"aaa") == 3);
  assert(std::char_traits<char8_t>::length(u8"aaaa") == 4);

  static_assert(test_constexpr(), "");
  return;
}
#else
int main(int, char**) { return 0; }
#endif
