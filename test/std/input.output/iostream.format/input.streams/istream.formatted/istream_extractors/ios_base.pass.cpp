// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostream_format_input_streams_istream_formatted_istream_extractors_ios_base).
// main -> test_input_output_iostream_format_input_streams_istream_formatted_istream_extractors_ios_base; file-scope helpers isolated in anon namespace.
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

// basic_istream<charT,traits>& operator>>(ios_base& (*pf)(ios_base&));

#include <istream>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostream_format_input_streams_istream_formatted_istream_extractors_ios_base { // libcis: isolate file-scope helpers
int f_called = 0;

std::ios_base&
f(std::ios_base& is)
{
    ++f_called;
    return is;
}
} using namespace libcis_ns_input_output_iostream_format_input_streams_istream_formatted_istream_extractors_ios_base; // libcis


void test_input_output_iostream_format_input_streams_istream_formatted_istream_extractors_ios_base()
{
    {
        std::istream is((std::streambuf*)0);
        is >> f;
        assert(f_called == 1);
    }

  return;
}
