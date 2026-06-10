// AST-transferred from libc++ by tools/transfer.py (slug=strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_eq).
// main -> test_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_eq; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <string>

// template<> struct char_traits<char16_t>

// static constexpr bool eq(char_type c1, char_type c2);

// UNSUPPORTED: c++03

#include <string>
#include <cassert>

void test_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_eq() {
  assert(std::char_traits<char16_t>::eq(u'a', u'a'));
  assert(!std::char_traits<char16_t>::eq(u'a', u'A'));
  return;
}
