// AST-transferred from libc++ by tools/transfer.py (slug=input_output_stream_buffers_streambuf_streambuf_protected_streambuf_put_area_pbump).
// main -> test_input_output_stream_buffers_streambuf_streambuf_protected_streambuf_put_area_pbump; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <streambuf>

// template <class charT, class traits = char_traits<charT> >
// class basic_streambuf;

// void pbump(int n);

#include <streambuf>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_stream_buffers_streambuf_streambuf_protected_streambuf_put_area_pbump { // libcis: isolate file-scope helpers
template <class CharT>
struct test
    : public std::basic_streambuf<CharT>
{
    typedef std::basic_streambuf<CharT> base;

    test() {}

    void setp(CharT* pbeg, CharT* pend)
    {
        base::setp(pbeg, pend);
    }

    void pbump(int n)
    {
        CharT* pbeg = base::pbase();
        CharT* pnext = base::pptr();
        CharT* pend = base::epptr();
        base::pbump(n);
        assert(base::pbase() == pbeg);
        assert(base::pptr() == pnext+n);
        assert(base::epptr() == pend);
    }
};
} using namespace libcis_ns_input_output_stream_buffers_streambuf_streambuf_protected_streambuf_put_area_pbump; // libcis


void test_input_output_stream_buffers_streambuf_streambuf_protected_streambuf_put_area_pbump()
{
    {
        test<char> t;
        char in[] = "ABCDE";
        t.setp(in, in+sizeof(in)/sizeof(in[0]));
        t.pbump(2);
        t.pbump(1);
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        test<wchar_t> t;
        wchar_t in[] = L"ABCDE";
        t.setp(in, in+sizeof(in)/sizeof(in[0]));
        t.pbump(3);
        t.pbump(1);
    }
#endif

  return;
}
