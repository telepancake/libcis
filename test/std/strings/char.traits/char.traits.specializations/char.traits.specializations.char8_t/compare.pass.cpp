// AST-transferred from libc++ by tools/transfer.py (slug=strings_char_traits_char_traits_specializations_char_traits_specializations_char8_t_compare).
// main -> test_strings_char_traits_char_traits_specializations_char_traits_specializations_char8_t_compare; file-scope helpers isolated in anon namespace.
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

// static constexpr int compare(const char_type* s1, const char_type* s2, size_t n);

#include <string>
#include <cassert>

#include "test_macros.h"

#ifndef TEST_HAS_NO_CHAR8_T
namespace libcis_ns_strings_char_traits_char_traits_specializations_char_traits_specializations_char8_t_compare { // libcis: isolate file-scope helpers
constexpr bool test_constexpr() {
  return std::char_traits<char8_t>::compare(u8"123", u8"223", 3) < 0 &&
         std::char_traits<char8_t>::compare(u8"223", u8"123", 3) > 0 &&
         std::char_traits<char8_t>::compare(u8"123", u8"123", 3) == 0;
}
} using namespace libcis_ns_strings_char_traits_char_traits_specializations_char_traits_specializations_char8_t_compare; // libcis


void test_strings_char_traits_char_traits_specializations_char_traits_specializations_char8_t_compare() {
  assert(std::char_traits<char8_t>::compare(u8"", u8"", 0) == 0);
  assert(std::char_traits<char8_t>::compare(NULL, NULL, 0) == 0);

  assert(std::char_traits<char8_t>::compare(u8"1", u8"1", 1) == 0);
  assert(std::char_traits<char8_t>::compare(u8"1", u8"2", 1) < 0);
  assert(std::char_traits<char8_t>::compare(u8"2", u8"1", 1) > 0);

  assert(std::char_traits<char8_t>::compare(u8"12", u8"12", 2) == 0);
  assert(std::char_traits<char8_t>::compare(u8"12", u8"13", 2) < 0);
  assert(std::char_traits<char8_t>::compare(u8"12", u8"22", 2) < 0);
  assert(std::char_traits<char8_t>::compare(u8"13", u8"12", 2) > 0);
  assert(std::char_traits<char8_t>::compare(u8"22", u8"12", 2) > 0);

  assert(std::char_traits<char8_t>::compare(u8"123", u8"123", 3) == 0);
  assert(std::char_traits<char8_t>::compare(u8"123", u8"223", 3) < 0);
  assert(std::char_traits<char8_t>::compare(u8"123", u8"133", 3) < 0);
  assert(std::char_traits<char8_t>::compare(u8"123", u8"124", 3) < 0);
  assert(std::char_traits<char8_t>::compare(u8"223", u8"123", 3) > 0);
  assert(std::char_traits<char8_t>::compare(u8"133", u8"123", 3) > 0);
  assert(std::char_traits<char8_t>::compare(u8"124", u8"123", 3) > 0);

  static_assert(test_constexpr(), "");
  return;
}
#else
int main(int, char**) { return 0; }
#endif
