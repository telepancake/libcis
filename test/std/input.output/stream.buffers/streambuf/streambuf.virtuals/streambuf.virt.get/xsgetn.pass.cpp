// AST-transferred from libc++ by tools/transfer.py (slug=input_output_stream_buffers_streambuf_streambuf_virtuals_streambuf_virt_get_xsgetn).
// main -> test_input_output_stream_buffers_streambuf_streambuf_virtuals_streambuf_virt_get_xsgetn; file-scope helpers isolated in anon namespace.
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

// streamsize xsgetn(char_type* s, streamsize n);

#include <streambuf>
#include <cassert>
#include <cstring>

#include "test_macros.h"

namespace libcis_ns_input_output_stream_buffers_streambuf_streambuf_virtuals_streambuf_virt_get_xsgetn { // libcis: isolate file-scope helpers
struct test
    : public std::basic_streambuf<char>
{
    typedef std::basic_streambuf<char> base;

    test() {}

    void setg(char* gbeg, char* gnext, char* gend)
    {
        base::setg(gbeg, gnext, gend);
    }
};
} using namespace libcis_ns_input_output_stream_buffers_streambuf_streambuf_virtuals_streambuf_virt_get_xsgetn; // libcis


void test_input_output_stream_buffers_streambuf_streambuf_virtuals_streambuf_virt_get_xsgetn()
{
    test t;
    char input[7] = "123456";
    t.setg(input, input, input+7);
    char output[sizeof(input)] = {0};
    assert(t.sgetn(output, 10) == 7);
    assert(std::strcmp(input, output) == 0);

  return;
}
