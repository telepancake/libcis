// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_basic_ios_members_fill_char_type).
// main -> test_input_output_iostreams_base_ios_basic_ios_members_fill_char_type; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// template <class charT, class traits> class basic_ios

// char_type fill(char_type fillch);

#include <ios>
#include <cassert>

#include "test_macros.h"

void test_input_output_iostreams_base_ios_basic_ios_members_fill_char_type()
{
    std::ios ios(0);
    char c = ios.fill('*');
    assert(c == ' ');
    assert(ios.fill() == '*');

  return;
}
