// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_stream_buffers_streambuf_streambuf_members_streambuf_buffer_pubsetbuf_e07c577a).
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

// basic_streambuf* pubsetbuf(char_type* s, streamsize n);

#include <streambuf>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_stream_buffers_streambuf_streambuf_members_streambuf_buffer_pubsetbuf_e07c577a { // libcis
template <class CharT>
struct test
    : public std::basic_streambuf<CharT>
{
    test() {}
};

int main(int, char**)
{
    {
        test<char> t;
        assert(t.pubsetbuf(0, 0) == &t);
    }

  return 0;

    return 0;
}
} // libcis_ns_input_output_stream_buffers_streambuf_streambuf_members_streambuf_buffer_pubsetbuf_e07c577a (libcis)

