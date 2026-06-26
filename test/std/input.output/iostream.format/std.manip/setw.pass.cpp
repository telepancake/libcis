// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_iostream_format_std_manip_setw_736dceae).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iomanip>

// <unspecified> setw(int n);

#include <iomanip>
#include <istream>
#include <ostream>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostream_format_std_manip_setw_736dceae { // libcis
template <class CharT>
struct testbuf
    : public std::basic_streambuf<CharT>
{
    testbuf() {}
};

int main(int, char**)
{
    {
        testbuf<char> sb;
        std::istream is(&sb);
        is >> std::setw(10);
        assert(is.width() == 10);
    }
    {
        testbuf<char> sb;
        std::ostream os(&sb);
        os << std::setw(10);
        assert(os.width() == 10);
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        testbuf<wchar_t> sb;
        std::wistream is(&sb);
        is >> std::setw(10);
        assert(is.width() == 10);
    }
    {
        testbuf<wchar_t> sb;
        std::wostream os(&sb);
        os << std::setw(10);
        assert(os.width() == 10);
    }
#endif

  return 0;

    return 0;
}
} // libcis_ns_input_output_iostream_format_std_manip_setw_736dceae (libcis)

