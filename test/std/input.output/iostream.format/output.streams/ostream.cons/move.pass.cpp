// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostream_format_output_streams_ostream_cons_move).
// main -> test_input_output_iostream_format_output_streams_ostream_cons_move; file-scope helpers isolated in anon namespace.
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

// basic_ostream(basic_ostream&& rhs);

#include <ostream>
#include <cassert>

#include "test_macros.h"


namespace libcis_ns_input_output_iostream_format_output_streams_ostream_cons_move { // libcis: isolate file-scope helpers
template <class CharT>
struct testbuf
    : public std::basic_streambuf<CharT>
{
    testbuf() {}
};

template <class CharT>
struct test_ostream
    : public std::basic_ostream<CharT>
{
    typedef std::basic_ostream<CharT> base;
    test_ostream(testbuf<CharT>* sb) : base(sb) {}

    test_ostream(test_ostream&& s)
        : base(std::move(s)) {}
};
} using namespace libcis_ns_input_output_iostream_format_output_streams_ostream_cons_move; // libcis



void test_input_output_iostream_format_output_streams_ostream_cons_move()
{
    {
        testbuf<char> sb;
        test_ostream<char> os1(&sb);
        test_ostream<char> os(std::move(os1));
        assert(os1.rdbuf() == &sb);
        assert(os.rdbuf() == 0);
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
        test_ostream<wchar_t> os1(&sb);
        test_ostream<wchar_t> os(std::move(os1));
        assert(os1.rdbuf() == &sb);
        assert(os.rdbuf() == 0);
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
