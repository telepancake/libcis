// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_stream_buffers_streambuf_streambuf_virtuals_streambuf_virt_put_xsputn_44c0d876).
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

// streamsize xsputn(const char_type* s, streamsize n);

#include <streambuf>
#include <cassert>
#include <cstring>

#include "test_macros.h"

namespace libcis_ns_input_output_stream_buffers_streambuf_streambuf_virtuals_streambuf_virt_put_xsputn_44c0d876 { // libcis
struct test
    : public std::basic_streambuf<char>
{
    typedef std::basic_streambuf<char> base;

    test() {}

    void setp(char* pbeg, char* pend)
    {
        base::setp(pbeg, pend);
    }
};

int main(int, char**)
{
    {
        test t;
        char in[] = "123456";
        assert(t.sputn(in, sizeof(in)) == 0);
        char out[sizeof(in)] = {0};
        t.setp(out, out+sizeof(out));
        assert(t.sputn(in, sizeof(in)) == sizeof(in));
        assert(std::strcmp(in, out) == 0);
    }

  return 0;

    return 0;
}
} // libcis_ns_input_output_stream_buffers_streambuf_streambuf_virtuals_streambuf_virt_put_xsputn_44c0d876 (libcis)

