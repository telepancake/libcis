// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_stream_buffers_streambuf_streambuf_members_streambuf_buffer_pubseekoff_2d7d3d6f).
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

// pos_type pubseekoff(off_type off, ios_base::seekdir way,
//                     ios_base::openmode which = ios_base::in | ios_base::out);

#include <streambuf>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_stream_buffers_streambuf_streambuf_members_streambuf_buffer_pubseekoff_2d7d3d6f { // libcis
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
        assert(t.pubseekoff(0, std::ios_base::beg) == -1);
        assert(t.pubseekoff(0, std::ios_base::beg, std::ios_base::app) == -1);
    }

  return 0;

    return 0;
}
} // libcis_ns_input_output_stream_buffers_streambuf_streambuf_members_streambuf_buffer_pubseekoff_2d7d3d6f (libcis)

