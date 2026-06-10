// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_basic_ios_cons_ctor_streambuf).
// main -> test_input_output_iostreams_base_ios_basic_ios_cons_ctor_streambuf; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// template <class charT, class traits> class basic_ios

// explicit basic_ios(basic_streambuf<charT,traits>* sb);

#include <ios>
#include <streambuf>
#include <cassert>

#include "test_macros.h"

void test_input_output_iostreams_base_ios_basic_ios_cons_ctor_streambuf()
{
    {
        std::streambuf* sb = 0;
        std::basic_ios<char> ios(sb);
        assert(ios.rdbuf() == sb);
        assert(ios.tie() == 0);
        assert(ios.rdstate() == std::ios::badbit);
        assert(ios.exceptions() == std::ios::goodbit);
        assert(ios.flags() == (std::ios::skipws | std::ios::dec));
        assert(ios.width() == 0);
        assert(ios.precision() == 6);
        assert(ios.fill() == ' ');
        assert(ios.getloc() == std::locale());
    }
    {
        std::streambuf* sb = (std::streambuf*)1;
        std::basic_ios<char> ios(sb);
        assert(ios.rdbuf() == sb);
        assert(ios.tie() == 0);
        assert(ios.rdstate() == std::ios::goodbit);
        assert(ios.exceptions() == std::ios::goodbit);
        assert(ios.flags() == (std::ios::skipws | std::ios::dec));
        assert(ios.width() == 0);
        assert(ios.precision() == 6);
        assert(ios.fill() == ' ');
        assert(ios.getloc() == std::locale());
    }

  return;
}
