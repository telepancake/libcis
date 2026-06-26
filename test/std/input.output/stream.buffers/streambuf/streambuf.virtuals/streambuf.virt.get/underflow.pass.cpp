// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_stream_buffers_streambuf_streambuf_virtuals_streambuf_virt_get_underflow_acdf5bab).
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

namespace libcis_ns_input_output_stream_buffers_streambuf_streambuf_virtuals_streambuf_virt_get_underflow_acdf5bab { // libcis
struct test
    : public std::basic_streambuf<char>
{
    test() {}
};

int main(int, char**)
{
    test t;
    assert(t.sgetc() == -1);

  return 0;

    return 0;
}
} // libcis_ns_input_output_stream_buffers_streambuf_streambuf_virtuals_streambuf_virt_get_underflow_acdf5bab (libcis)

