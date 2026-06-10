// AST-transferred from libc++ by tools/transfer.py (slug=strings_char_traits_char_traits_specializations_char_traits_specializations_char8_t_eq_int_type).
// main -> test_strings_char_traits_char_traits_specializations_char_traits_specializations_char8_t_eq_int_type; file-scope helpers isolated in anon namespace.
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

// static constexpr bool eq_int_type(int_type c1, int_type c2);

#include <string>
#include <cassert>

#include "test_macros.h"

void test_strings_char_traits_char_traits_specializations_char_traits_specializations_char8_t_eq_int_type() {
#ifndef TEST_HAS_NO_CHAR8_T
  assert(std::char_traits<char8_t>::eq_int_type(u8'a', u8'a'));
  assert(!std::char_traits<char8_t>::eq_int_type(u8'a', u8'A'));
  assert(!std::char_traits<char8_t>::eq_int_type(std::char_traits<char8_t>::eof(), u8'A'));
  assert(std::char_traits<char8_t>::eq_int_type(std::char_traits<char8_t>::eof(), std::char_traits<char8_t>::eof()));
#endif

  return;
}
