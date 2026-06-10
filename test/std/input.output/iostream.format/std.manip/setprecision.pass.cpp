// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostream_format_std_manip_setprecision).
// main -> test_input_output_iostream_format_std_manip_setprecision; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iomanip>

// T5 setprecision(int n);

#include <iomanip>
#include <istream>
#include <ostream>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostream_format_std_manip_setprecision { // libcis: isolate file-scope helpers
template <class CharT>
struct testbuf
    : public std::basic_streambuf<CharT>
{
    testbuf() {}
};
} using namespace libcis_ns_input_output_iostream_format_std_manip_setprecision; // libcis


void test_input_output_iostream_format_std_manip_setprecision()
{
    {
        testbuf<char> sb;
        std::istream is(&sb);
        is >> std::setprecision(10);
        assert(is.precision() == 10);
    }
    {
        testbuf<char> sb;
        std::ostream os(&sb);
        os << std::setprecision(10);
        assert(os.precision() == 10);
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        testbuf<wchar_t> sb;
        std::wistream is(&sb);
        is >> std::setprecision(10);
        assert(is.precision() == 10);
    }
    {
        testbuf<wchar_t> sb;
        std::wostream os(&sb);
        os << std::setprecision(10);
        assert(os.precision() == 10);
    }
#endif

  return;
}
