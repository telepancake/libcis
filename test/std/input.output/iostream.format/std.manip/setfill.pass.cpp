// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_iostream_format_std_manip_setfill_1d4dea6b).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iomanip>

// template<charT> T4 setfill(charT c);

#include <iomanip>
#include <ostream>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostream_format_std_manip_setfill_1d4dea6b { // libcis
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
        std::ostream os(&sb);
        os << std::setfill('*');
        assert(os.fill() == '*');
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        testbuf<wchar_t> sb;
        std::wostream os(&sb);
        os << std::setfill(L'*');
        assert(os.fill() == L'*');
    }
#endif

  return 0;

    return 0;
}
} // libcis_ns_input_output_iostream_format_std_manip_setfill_1d4dea6b (libcis)

