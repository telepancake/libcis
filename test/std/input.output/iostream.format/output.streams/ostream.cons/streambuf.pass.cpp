// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostream_format_output_streams_ostream_cons_streambuf).
// main -> test_input_output_iostream_format_output_streams_ostream_cons_streambuf; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ostream>

// template <class charT, class traits = char_traits<charT> >
// class basic_ostream;

// explicit basic_ostream(basic_streambuf<charT,traits>* sb);

#include <ostream>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostream_format_output_streams_ostream_cons_streambuf { // libcis: isolate file-scope helpers
template <class CharT>
struct testbuf
    : public std::basic_streambuf<CharT>
{
    testbuf() {}
};
} using namespace libcis_ns_input_output_iostream_format_output_streams_ostream_cons_streambuf; // libcis


void test_input_output_iostream_format_output_streams_ostream_cons_streambuf()
{
    {
        testbuf<char> sb;
        std::basic_ostream<char> os(&sb);
        assert(os.rdbuf() == &sb);
        assert(os.tie() == 0);
        assert(os.fill() == ' ');
        assert(os.rdstate() == os.goodbit);
        assert(os.exceptions() == os.goodbit);
        assert(os.flags() == (os.skipws | os.dec));
        assert(os.precision() == 6);
        assert(os.getloc().name() == "C");
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        testbuf<wchar_t> sb;
        std::basic_ostream<wchar_t> os(&sb);
        assert(os.rdbuf() == &sb);
        assert(os.tie() == 0);
        assert(os.fill() == L' ');
        assert(os.rdstate() == os.goodbit);
        assert(os.exceptions() == os.goodbit);
        assert(os.flags() == (os.skipws | os.dec));
        assert(os.precision() == 6);
        assert(os.getloc().name() == "C");
    }
#endif

  return;
}
