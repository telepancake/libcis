// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_iostream_format_output_streams_ostream_cons_streambuf_a423212b).
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

namespace libcis_ns_input_output_iostream_format_output_streams_ostream_cons_streambuf_a423212b { // libcis
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

  return 0;

    return 0;
}
} // libcis_ns_input_output_iostream_format_output_streams_ostream_cons_streambuf_a423212b (libcis)

