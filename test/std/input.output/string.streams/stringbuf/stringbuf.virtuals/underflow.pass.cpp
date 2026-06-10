// AST-transferred from libc++ by tools/transfer.py (slug=input_output_string_streams_stringbuf_stringbuf_virtuals_underflow).
// main -> test_input_output_string_streams_stringbuf_stringbuf_virtuals_underflow; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <sstream>

// template <class charT, class traits = char_traits<charT>, class Allocator = allocator<charT> >
// class basic_stringbuf

// int_type underflow();

#include <sstream>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_string_streams_stringbuf_stringbuf_virtuals_underflow { // libcis: isolate file-scope helpers
template <class CharT>
struct testbuf
    : public std::basic_stringbuf<CharT>
{
    typedef std::basic_stringbuf<CharT> base;
    explicit testbuf(const std::basic_string<CharT>& str)
        : base(str) {}

    typename base::int_type underflow() {return base::underflow();}
    void pbump(int n) {base::pbump(n);}
};
} using namespace libcis_ns_input_output_string_streams_stringbuf_stringbuf_virtuals_underflow; // libcis


void test_input_output_string_streams_stringbuf_stringbuf_virtuals_underflow()
{
    {
        testbuf<char> sb("123");
        sb.pbump(3);
        assert(sb.underflow() == '1');
        assert(sb.underflow() == '1');
        assert(sb.snextc() == '2');
        assert(sb.underflow() == '2');
        assert(sb.underflow() == '2');
        assert(sb.snextc() == '3');
        assert(sb.underflow() == '3');
        assert(sb.underflow() == '3');
        assert(sb.snextc() == std::char_traits<char>::eof());
        assert(sb.underflow() == std::char_traits<char>::eof());
        assert(sb.underflow() == std::char_traits<char>::eof());
        sb.sputc('4');
        assert(sb.underflow() == '4');
        assert(sb.underflow() == '4');
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        testbuf<wchar_t> sb(L"123");
        sb.pbump(3);
        assert(sb.underflow() == L'1');
        assert(sb.underflow() == L'1');
        assert(sb.snextc() == L'2');
        assert(sb.underflow() == L'2');
        assert(sb.underflow() == L'2');
        assert(sb.snextc() == L'3');
        assert(sb.underflow() == L'3');
        assert(sb.underflow() == L'3');
        assert(sb.snextc() == std::char_traits<wchar_t>::eof());
        assert(sb.underflow() == std::char_traits<wchar_t>::eof());
        assert(sb.underflow() == std::char_traits<wchar_t>::eof());
        sb.sputc(L'4');
        assert(sb.underflow() == L'4');
        assert(sb.underflow() == L'4');
    }
#endif // TEST_HAS_NO_WIDE_CHARACTERS

  return;
}
