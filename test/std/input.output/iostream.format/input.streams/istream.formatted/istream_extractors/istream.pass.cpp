// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostream_format_input_streams_istream_formatted_istream_extractors_istream).
// main -> test_input_output_iostream_format_input_streams_istream_formatted_istream_extractors_istream; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <istream>

// template <class charT, class traits = char_traits<charT> >
//   class basic_istream;

// basic_istream<charT,traits>& operator>>(basic_istream<charT,traits>&
//                                         (*pf)(basic_istream<charT,traits>&));

#include <istream>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostream_format_input_streams_istream_formatted_istream_extractors_istream { // libcis: isolate file-scope helpers
int f_called = 0;

template <class CharT>
std::basic_istream<CharT>&
f(std::basic_istream<CharT>& is)
{
    ++f_called;
    return is;
}
} using namespace libcis_ns_input_output_iostream_format_input_streams_istream_formatted_istream_extractors_istream; // libcis


void test_input_output_iostream_format_input_streams_istream_formatted_istream_extractors_istream()
{
    {
        std::istream is((std::streambuf*)0);
        is >> f;
        assert(f_called == 1);
    }

  return;
}
