// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_iostream_format_std_manip_resetiosflags_2027cacd).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iomanip>

// T1 resetiosflags(ios_base::fmtflags mask);

#include <iomanip>
#include <istream>
#include <ostream>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostream_format_std_manip_resetiosflags_2027cacd { // libcis
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
        assert(is.flags() & std::ios_base::skipws);
        is >> std::resetiosflags(std::ios_base::skipws);
        assert(!(is.flags() & std::ios_base::skipws));
    }
    {
        testbuf<char> sb;
        std::ostream os(&sb);
        assert(os.flags() & std::ios_base::skipws);
        os << std::resetiosflags(std::ios_base::skipws);
        assert(!(os.flags() & std::ios_base::skipws));
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        testbuf<wchar_t> sb;
        std::wistream is(&sb);
        assert(is.flags() & std::ios_base::skipws);
        is >> std::resetiosflags(std::ios_base::skipws);
        assert(!(is.flags() & std::ios_base::skipws));
    }
    {
        testbuf<wchar_t> sb;
        std::wostream os(&sb);
        assert(os.flags() & std::ios_base::skipws);
        os << std::resetiosflags(std::ios_base::skipws);
        assert(!(os.flags() & std::ios_base::skipws));
    }
#endif

  return 0;

    return 0;
}
} // libcis_ns_input_output_iostream_format_std_manip_resetiosflags_2027cacd (libcis)

