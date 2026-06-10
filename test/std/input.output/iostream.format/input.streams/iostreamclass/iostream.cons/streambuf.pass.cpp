// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostream_format_input_streams_iostreamclass_iostream_cons_streambuf).
// main -> test_input_output_iostream_format_input_streams_iostreamclass_iostream_cons_streambuf; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <istream>

// template <class charT, class traits = char_traits<charT> >
// class basic_iostream;

// explicit basic_iostream(basic_streambuf<charT,traits>* sb);

#include <istream>
#include <cassert>
#include <streambuf>

#include "test_macros.h"

namespace libcis_ns_input_output_iostream_format_input_streams_iostreamclass_iostream_cons_streambuf { // libcis: isolate file-scope helpers
template <class CharT>
struct testbuf
    : public std::basic_streambuf<CharT>
{
    testbuf() {}
};
} using namespace libcis_ns_input_output_iostream_format_input_streams_iostreamclass_iostream_cons_streambuf; // libcis


void test_input_output_iostream_format_input_streams_iostreamclass_iostream_cons_streambuf()
{
    {
        testbuf<char> sb;
        std::basic_iostream<char> is(&sb);
        assert(is.rdbuf() == &sb);
        assert(is.tie() == 0);
        assert(is.fill() == ' ');
        assert(is.rdstate() == is.goodbit);
        assert(is.exceptions() == is.goodbit);
        assert(is.flags() == (is.skipws | is.dec));
        assert(is.precision() == 6);
        assert(is.getloc().name() == "C");
        assert(is.gcount() == 0);
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        testbuf<wchar_t> sb;
        std::basic_iostream<wchar_t> is(&sb);
        assert(is.rdbuf() == &sb);
        assert(is.tie() == 0);
        assert(is.fill() == L' ');
        assert(is.rdstate() == is.goodbit);
        assert(is.exceptions() == is.goodbit);
        assert(is.flags() == (is.skipws | is.dec));
        assert(is.precision() == 6);
        assert(is.getloc().name() == "C");
        assert(is.gcount() == 0);
    }
#endif

  return;
}
