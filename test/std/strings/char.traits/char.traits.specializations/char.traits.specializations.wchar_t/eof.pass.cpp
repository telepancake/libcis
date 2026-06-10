// AST-transferred from libc++ by tools/transfer.py (slug=strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_eof).
// main -> test_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_eof; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <string>

// template<> struct char_traits<wchar_t>

// static constexpr int_type eof();

// UNSUPPORTED: no-wide-characters

#include <string>
#include <cassert>

#include "test_macros.h"

void test_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_eof() {
  assert(std::char_traits<wchar_t>::eof() == WEOF);

  return;
}
