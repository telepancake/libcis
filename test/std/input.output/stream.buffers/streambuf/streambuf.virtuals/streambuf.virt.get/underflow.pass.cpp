// AST-transferred from libc++ by tools/transfer.py (slug=input_output_stream_buffers_streambuf_streambuf_virtuals_streambuf_virt_get_underflow).
// main -> test_input_output_stream_buffers_streambuf_streambuf_virtuals_streambuf_virt_get_underflow; file-scope helpers isolated in anon namespace.
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

// int_type underflow();

#include <streambuf>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_stream_buffers_streambuf_streambuf_virtuals_streambuf_virt_get_underflow { // libcis: isolate file-scope helpers
struct test
    : public std::basic_streambuf<char>
{
    test() {}
};
} using namespace libcis_ns_input_output_stream_buffers_streambuf_streambuf_virtuals_streambuf_virt_get_underflow; // libcis


void test_input_output_stream_buffers_streambuf_streambuf_virtuals_streambuf_virt_get_underflow()
{
    test t;
    assert(t.sgetc() == -1);

  return;
}
