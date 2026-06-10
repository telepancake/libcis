// AST-transferred from libc++ by tools/transfer.py (slug=strings_char_traits_char_traits_specializations_char_traits_specializations_char_to_int_type).
// main -> test_strings_char_traits_char_traits_specializations_char_traits_specializations_char_to_int_type; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <string>

// template<> struct char_traits<char>

// static constexpr int_type to_int_type(char_type c);

#include <string>
#include <cassert>

#include "test_macros.h"

void test_strings_char_traits_char_traits_specializations_char_traits_specializations_char_to_int_type() {
  assert(std::char_traits<char>::to_int_type('a') == 'a');
  assert(std::char_traits<char>::to_int_type('A') == 'A');
  assert(std::char_traits<char>::to_int_type(0) == 0);

  return;
}
