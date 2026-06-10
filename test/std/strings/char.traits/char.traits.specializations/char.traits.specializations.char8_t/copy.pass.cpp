// AST-transferred from libc++ by tools/transfer.py (slug=strings_char_traits_char_traits_specializations_char_traits_specializations_char8_t_copy).
// main -> test_strings_char_traits_char_traits_specializations_char_traits_specializations_char8_t_copy; file-scope helpers isolated in anon namespace.
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

// static char_type* copy(char_type* s1, const char_type* s2, size_t n);

#include <string>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_strings_char_traits_char_traits_specializations_char_traits_specializations_char8_t_copy { // libcis: isolate file-scope helpers
TEST_CONSTEXPR_CXX20 bool test() {
#ifndef TEST_HAS_NO_CHAR8_T
  char8_t s1[]  = {1, 2, 3};
  char8_t s2[3] = {0};
  assert(std::char_traits<char8_t>::copy(s2, s1, 3) == s2);
  assert(s2[0] == char8_t(1));
  assert(s2[1] == char8_t(2));
  assert(s2[2] == char8_t(3));
  assert(std::char_traits<char8_t>::copy(NULL, s1, 0) == NULL);
  assert(std::char_traits<char8_t>::copy(s1, NULL, 0) == s1);
#endif

  return true;
}
} using namespace libcis_ns_strings_char_traits_char_traits_specializations_char_traits_specializations_char8_t_copy; // libcis


void test_strings_char_traits_char_traits_specializations_char_traits_specializations_char8_t_copy() {
  test();

#if TEST_STD_VER > 17
  static_assert(test());
#endif

  return;
}
