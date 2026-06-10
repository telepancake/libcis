// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_iostate_flags_exceptions_iostate).
// main -> test_input_output_iostreams_base_ios_iostate_flags_exceptions_iostate; file-scope helpers isolated in anon namespace.
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

namespace libcis_ns_input_output_iostreams_base_ios_iostate_flags_exceptions_iostate { // libcis: isolate file-scope helpers
struct testbuf : public std::streambuf {};
} using namespace libcis_ns_input_output_iostreams_base_ios_iostate_flags_exceptions_iostate; // libcis


void test_input_output_iostreams_base_ios_iostate_flags_exceptions_iostate()
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

  return;
}
