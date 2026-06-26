// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_iostate_flags_exceptions_iostate_f60b7fea).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// template <class charT, class traits> class basic_ios

// iostate exceptions() const;

#include <ios>
#include <streambuf>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostreams_base_ios_iostate_flags_exceptions_iostate_f60b7fea { // libcis
struct testbuf : public std::streambuf {};

int main(int, char**)
{
    {
        std::ios ios(0);
        assert(ios.exceptions() == std::ios::goodbit);
        ios.exceptions(std::ios::eofbit);
        assert(ios.exceptions() == std::ios::eofbit);
#ifndef TEST_HAS_NO_EXCEPTIONS
        try
        {
            ios.exceptions(std::ios::badbit);
            assert(false);
        }
        catch (std::ios::failure&)
        {
        }
        assert(ios.exceptions() == std::ios::badbit);
#endif
    }
    {
        testbuf sb;
        std::ios ios(&sb);
        assert(ios.exceptions() == std::ios::goodbit);
        ios.exceptions(std::ios::eofbit);
        assert(ios.exceptions() == std::ios::eofbit);
        ios.exceptions(std::ios::badbit);
        assert(ios.exceptions() == std::ios::badbit);
    }

  return 0;

    return 0;
}
} // libcis_ns_input_output_iostreams_base_ios_iostate_flags_exceptions_iostate_f60b7fea (libcis)

