// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_basic_ios_members_narrow).
// main -> test_input_output_iostreams_base_ios_basic_ios_members_narrow; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// template <class charT, class traits> class basic_ios

// char narrow(char_type c, char dfault) const;

// XFAIL: no-wide-characters

#include <ios>
#include <cassert>

#include "test_macros.h"

void test_input_output_iostreams_base_ios_basic_ios_members_narrow()
{
    const std::wios ios(0);
    assert(ios.narrow(L'c', '*') == 'c');
    assert(ios.narrow(L'\u203C', '*') == '*');

  return;
}
