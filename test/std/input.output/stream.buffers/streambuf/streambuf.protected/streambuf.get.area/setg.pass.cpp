// AST-transferred from libc++ by tools/transfer.py (slug=input_output_stream_buffers_streambuf_streambuf_protected_streambuf_get_area_setg).
// main -> test_input_output_stream_buffers_streambuf_streambuf_protected_streambuf_get_area_setg; file-scope helpers isolated in anon namespace.
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

// void setg(char_type* gbeg, char_type* gnext, char_type* gend);

#include <streambuf>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_stream_buffers_streambuf_streambuf_protected_streambuf_get_area_setg { // libcis: isolate file-scope helpers
template <class CharT>
struct test
    : public std::basic_streambuf<CharT>
{
    typedef std::basic_streambuf<CharT> base;

    test() {}

    void setg(CharT* gbeg, CharT* gnext, CharT* gend)
    {
        base::setg(gbeg, gnext, gend);
        assert(base::eback() == gbeg);
        assert(base::gptr() == gnext);
        assert(base::egptr() == gend);
    }
};
} using namespace libcis_ns_input_output_stream_buffers_streambuf_streambuf_protected_streambuf_get_area_setg; // libcis


void test_input_output_stream_buffers_streambuf_streambuf_protected_streambuf_get_area_setg()
{
    {
        test<char> t;
        char in[] = "ABC";
        t.setg(in, in+1, in+sizeof(in)/sizeof(in[0]));
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        test<wchar_t> t;
        wchar_t in[] = L"ABC";
        t.setg(in, in+1, in+sizeof(in)/sizeof(in[0]));
    }
#endif

  return;
}
