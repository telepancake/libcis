// AST-transferred from libc++ by tools/transfer.py (slug=strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_assign3).
// main -> test_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_assign3; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <string>

// template<> struct char_traits<char32_t>

// static char_type* assign(char_type* s, size_t n, char_type a);

#include <string>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_assign3 { // libcis: isolate file-scope helpers
TEST_CONSTEXPR_CXX20 bool test() {
  char32_t s2[3] = {0};
  assert(std::char_traits<char32_t>::assign(s2, 3, char32_t(5)) == s2);
  assert(s2[0] == char32_t(5));
  assert(s2[1] == char32_t(5));
  assert(s2[2] == char32_t(5));
  assert(std::char_traits<char32_t>::assign(NULL, 0, char32_t(5)) == NULL);

  return true;
}
} using namespace libcis_ns_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_assign3; // libcis


void test_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_assign3() {
  test();

#if TEST_STD_VER > 17
  static_assert(test());
#endif

  return;
}
