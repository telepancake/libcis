// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostream_format_output_streams_ostream_manip_flush).
// main -> test_input_output_iostream_format_output_streams_ostream_manip_flush; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ostream>

// template <class charT, class traits = char_traits<charT> >
//   class basic_ostream;

// template <class charT, class traits>
//   basic_ostream<charT,traits>& flush(basic_ostream<charT,traits>& os);

#include <ostream>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostream_format_output_streams_ostream_manip_flush { // libcis: isolate file-scope helpers
int sync_called = 0;

template <class CharT>
class testbuf
    : public std::basic_streambuf<CharT>
{
public:
    testbuf()
    {
    }

protected:

    virtual int
        sync()
        {
            ++sync_called;
            return 0;
        }
};
} using namespace libcis_ns_input_output_iostream_format_output_streams_ostream_manip_flush; // libcis


void test_input_output_iostream_format_output_streams_ostream_manip_flush()
{
    {
        testbuf<char> sb;
        std::ostream os(&sb);
        std::flush(os);
        assert(sync_called == 1);
        assert(os.good());
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        testbuf<wchar_t> sb;
        std::wostream os(&sb);
        std::flush(os);
        assert(sync_called == 2);
        assert(os.good());
    }
#endif

  return;
}
