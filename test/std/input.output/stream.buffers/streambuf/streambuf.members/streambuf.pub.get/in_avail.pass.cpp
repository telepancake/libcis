// AST-transferred from libc++ by tools/transfer.py (slug=input_output_stream_buffers_streambuf_streambuf_members_streambuf_pub_get_in_avail).
// main -> test_input_output_stream_buffers_streambuf_streambuf_members_streambuf_pub_get_in_avail; file-scope helpers isolated in anon namespace.
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

// streamsize in_avail();

#include <streambuf>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_stream_buffers_streambuf_streambuf_members_streambuf_pub_get_in_avail { // libcis: isolate file-scope helpers
int showmanyc_called = 0;

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
protected:
    std::streamsize showmanyc()
    {
        ++showmanyc_called;
        return 5;
    }
};
} using namespace libcis_ns_input_output_stream_buffers_streambuf_streambuf_members_streambuf_pub_get_in_avail; // libcis


void test_input_output_stream_buffers_streambuf_streambuf_members_streambuf_pub_get_in_avail()
{
    {
        test<char> t;
        assert(t.in_avail() == 5);
        assert(showmanyc_called == 1);
        char in[5];
        t.setg(in, in+2, in+5);
        assert(t.in_avail() == 3);
    }

  return;
}
