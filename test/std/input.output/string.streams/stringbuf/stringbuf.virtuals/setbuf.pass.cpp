// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_string_streams_stringbuf_stringbuf_virtuals_setbuf_6f6f6f00).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <sstream>

// template <class charT, class traits = char_traits<charT>, class Allocator = allocator<charT> >
// class basic_stringbuf

// basic_streambuf<charT,traits>* setbuf(charT* s, streamsize n);

#include <sstream>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_string_streams_stringbuf_stringbuf_virtuals_setbuf_6f6f6f00 { // libcis
int main(int, char**)
{
    {
        std::stringbuf sb("0123456789");
        assert(sb.pubsetbuf(0, 0) == &sb);
        assert(sb.str() == "0123456789");
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        std::wstringbuf sb(L"0123456789");
        assert(sb.pubsetbuf(0, 0) == &sb);
        assert(sb.str() == L"0123456789");
    }
#endif

  return 0;

    return 0;
}
} // libcis_ns_input_output_string_streams_stringbuf_stringbuf_virtuals_setbuf_6f6f6f00 (libcis)

