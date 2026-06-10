// AST-transferred from libc++ by tools/transfer.py (slug=strings_char_traits_char_traits_specializations_char_traits_specializations_char8_t_eof).
// main -> test_strings_char_traits_char_traits_specializations_char_traits_specializations_char8_t_eof; file-scope helpers isolated in anon namespace.
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

// static constexpr int_type eof();

#include <string>
#include <cassert>

#include "test_macros.h"

void test_strings_char_traits_char_traits_specializations_char_traits_specializations_char8_t_eof() {
#ifndef TEST_HAS_NO_CHAR8_T
  std::char_traits<char8_t>::int_type i = std::char_traits<char8_t>::eof();
  ((void)i); // Prevent unused warning
#endif

  return;
}
