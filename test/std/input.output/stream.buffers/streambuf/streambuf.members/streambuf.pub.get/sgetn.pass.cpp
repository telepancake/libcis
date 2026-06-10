// AST-transferred from libc++ by tools/transfer.py (slug=input_output_stream_buffers_streambuf_streambuf_members_streambuf_pub_get_sgetn).
// main -> test_input_output_stream_buffers_streambuf_streambuf_members_streambuf_pub_get_sgetn; file-scope helpers isolated in anon namespace.
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

// streamsize sgetn(char_type* s, streamsize n);

#include <streambuf>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_stream_buffers_streambuf_streambuf_members_streambuf_pub_get_sgetn { // libcis: isolate file-scope helpers
int xsgetn_called = 0;

struct test
    : public std::basic_streambuf<char>
{
    test() {}

protected:
    std::streamsize xsgetn(char_type*, std::streamsize)
    {
        ++xsgetn_called;
        return 10;
    }
};
} using namespace libcis_ns_input_output_stream_buffers_streambuf_streambuf_members_streambuf_pub_get_sgetn; // libcis


void test_input_output_stream_buffers_streambuf_streambuf_members_streambuf_pub_get_sgetn()
{
    test t;
    assert(xsgetn_called == 0);
    assert(t.sgetn(0, 0) == 10);
    assert(xsgetn_called == 1);

  return;
}
