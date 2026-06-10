// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_basic_ios_members_tie).
// main -> test_input_output_iostreams_base_ios_basic_ios_members_tie; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// template <class charT, class traits> class basic_ios

// basic_ostream<charT,traits>* tie() const;

#include <ios>
#include <cassert>

#include "test_macros.h"

void test_input_output_iostreams_base_ios_basic_ios_members_tie()
{
    const std::basic_ios<char> ios(0);
    assert(ios.tie() == 0);

  return;
}
