// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostream_format_std_manip_setfill).
// main -> test_input_output_iostream_format_std_manip_setfill; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iomanip>

// template<charT> T4 setfill(charT c);

#include <iomanip>
#include <ostream>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostream_format_std_manip_setfill { // libcis: isolate file-scope helpers
template <class CharT>
struct testbuf
    : public std::basic_streambuf<CharT>
{
    testbuf() {}
};
} using namespace libcis_ns_input_output_iostream_format_std_manip_setfill; // libcis


void test_input_output_iostream_format_std_manip_setfill()
{
    {
        testbuf<char> sb;
        std::ostream os(&sb);
        os << std::setfill('*');
        assert(os.fill() == '*');
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        testbuf<wchar_t> sb;
        std::wostream os(&sb);
        os << std::setfill(L'*');
        assert(os.fill() == L'*');
    }
#endif

  return;
}
