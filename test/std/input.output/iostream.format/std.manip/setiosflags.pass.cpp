// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostream_format_std_manip_setiosflags).
// main -> test_input_output_iostream_format_std_manip_setiosflags; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iomanip>

// T2 setiosflags (ios_base::fmtflags mask);

#include <iomanip>
#include <istream>
#include <ostream>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostream_format_std_manip_setiosflags { // libcis: isolate file-scope helpers
template <class CharT>
struct testbuf
    : public std::basic_streambuf<CharT>
{
    testbuf() {}
};
} using namespace libcis_ns_input_output_iostream_format_std_manip_setiosflags; // libcis


void test_input_output_iostream_format_std_manip_setiosflags()
{
    {
        testbuf<char> sb;
        std::istream is(&sb);
        assert(!(is.flags() & std::ios_base::oct));
        is >> std::setiosflags(std::ios_base::oct);
        assert(is.flags() & std::ios_base::oct);
    }
    {
        testbuf<char> sb;
        std::ostream os(&sb);
        assert(!(os.flags() & std::ios_base::oct));
        os << std::setiosflags(std::ios_base::oct);
        assert(os.flags() & std::ios_base::oct);
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        testbuf<wchar_t> sb;
        std::wistream is(&sb);
        assert(!(is.flags() & std::ios_base::oct));
        is >> std::setiosflags(std::ios_base::oct);
        assert(is.flags() & std::ios_base::oct);
    }
    {
        testbuf<wchar_t> sb;
        std::wostream os(&sb);
        assert(!(os.flags() & std::ios_base::oct));
        os << std::setiosflags(std::ios_base::oct);
        assert(os.flags() & std::ios_base::oct);
    }
#endif

  return;
}
