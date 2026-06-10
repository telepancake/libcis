// AST-transferred from libc++ by tools/transfer.py (slug=strings_char_traits_char_traits_specializations_char_traits_specializations_char8_t_find).
// main -> test_strings_char_traits_char_traits_specializations_char_traits_specializations_char8_t_find; file-scope helpers isolated in anon namespace.
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

// static constexpr const char_type* find(const char_type* s, size_t n, const char_type& a);

#include <string>
#include <cassert>

#include "test_macros.h"

#ifndef TEST_HAS_NO_CHAR8_T
namespace libcis_ns_strings_char_traits_char_traits_specializations_char_traits_specializations_char8_t_find { // libcis: isolate file-scope helpers
constexpr bool test_constexpr() {
  constexpr const char8_t* p = u8"123";
  return std::char_traits<char8_t>::find(p, 3, u8'1') == p && std::char_traits<char8_t>::find(p, 3, u8'2') == p + 1 &&
         std::char_traits<char8_t>::find(p, 3, u8'3') == p + 2 &&
         std::char_traits<char8_t>::find(p, 3, u8'4') == nullptr;
}
} using namespace libcis_ns_strings_char_traits_char_traits_specializations_char_traits_specializations_char8_t_find; // libcis


void test_strings_char_traits_char_traits_specializations_char_traits_specializations_char8_t_find() {
  char8_t s1[] = {1, 2, 3};
  assert(std::char_traits<char8_t>::find(s1, 3, char8_t(1)) == s1);
  assert(std::char_traits<char8_t>::find(s1, 3, char8_t(2)) == s1 + 1);
  assert(std::char_traits<char8_t>::find(s1, 3, char8_t(3)) == s1 + 2);
  assert(std::char_traits<char8_t>::find(s1, 3, char8_t(4)) == 0);
  assert(std::char_traits<char8_t>::find(s1, 3, char8_t(0)) == 0);
  assert(std::char_traits<char8_t>::find(NULL, 0, char8_t(0)) == 0);

  static_assert(test_constexpr(), "");
  return;
}
#else
int main(int, char**) { return 0; }
#endif
