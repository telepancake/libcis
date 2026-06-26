// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_iostream_format_input_streams_istream_istream_cons_streambuf_f0a7a18e).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <istream>

// template <class charT, class traits = char_traits<charT> >
// class basic_istream;

// explicit basic_istream(basic_streambuf<charT,traits>* sb);

#include <istream>
#include <cassert>
#include <streambuf>

#include "test_macros.h"

namespace libcis_ns_input_output_iostream_format_input_streams_istream_istream_cons_streambuf_f0a7a18e { // libcis
template <class CharT>
struct testbuf
    : public std::basic_streambuf<CharT>
{
    testbuf() {}
};

int main(int, char**)
{
    {
        testbuf<char> sb;
        std::basic_istream<char> is(&sb);
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
        std::basic_istream<wchar_t> is(&sb);
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

  return 0;

    return 0;
}
} // libcis_ns_input_output_iostream_format_input_streams_istream_istream_cons_streambuf_f0a7a18e (libcis)

