// AST-transferred from libc++ by tools/transfer.py (slug=input_output_stream_buffers_streambuf_streambuf_protected_streambuf_get_area_gbump).
// main -> test_input_output_stream_buffers_streambuf_streambuf_protected_streambuf_get_area_gbump; file-scope helpers isolated in anon namespace.
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

// void gbump(int n);

#include <streambuf>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_stream_buffers_streambuf_streambuf_protected_streambuf_get_area_gbump { // libcis: isolate file-scope helpers
template <class CharT>
struct test
    : public std::basic_streambuf<CharT>
{
    typedef std::basic_streambuf<CharT> base;

    test() {}

    void setg(CharT* gbeg, CharT* gnext, CharT* gend)
    {
        base::setg(gbeg, gnext, gend);
    }

    void gbump(int n)
    {
        CharT* gbeg = base::eback();
        CharT* gnext = base::gptr();
        CharT* gend = base::egptr();
        base::gbump(n);
        assert(base::eback() == gbeg);
        assert(base::gptr() == gnext+n);
        assert(base::egptr() == gend);
    }
};
} using namespace libcis_ns_input_output_stream_buffers_streambuf_streambuf_protected_streambuf_get_area_gbump; // libcis


void test_input_output_stream_buffers_streambuf_streambuf_protected_streambuf_get_area_gbump()
{
    {
        test<char> t;
        char in[] = "ABCDE";
        t.setg(in, in+1, in+sizeof(in)/sizeof(in[0]));
        t.gbump(2);
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        test<wchar_t> t;
        wchar_t in[] = L"ABCDE";
        t.setg(in, in+1, in+sizeof(in)/sizeof(in[0]));
        t.gbump(3);
    }
#endif

  return;
}
