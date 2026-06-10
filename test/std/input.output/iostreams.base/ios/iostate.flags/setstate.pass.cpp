// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_iostate_flags_setstate).
// main -> test_input_output_iostreams_base_ios_iostate_flags_setstate; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// template <class charT, class traits> class basic_ios

// void setstate(iostate state);

#include <ios>
#include <streambuf>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostreams_base_ios_iostate_flags_setstate { // libcis: isolate file-scope helpers
struct testbuf : public std::streambuf {};
} using namespace libcis_ns_input_output_iostreams_base_ios_iostate_flags_setstate; // libcis


void test_input_output_iostreams_base_ios_iostate_flags_setstate()
{
    {
        std::ios ios(0);
        ios.setstate(std::ios::goodbit);
        assert(ios.rdstate() == std::ios::badbit);
#ifndef TEST_HAS_NO_EXCEPTIONS
        try
        {
            ios.exceptions(std::ios::badbit);
            assert(false);
        }
        catch (...)
        {
        }
        try
        {
            ios.setstate(std::ios::goodbit);
            assert(false);
        }
        catch (std::ios::failure&)
        {
            assert(ios.rdstate() == std::ios::badbit);
        }
        try
        {
            ios.setstate(std::ios::eofbit);
            assert(false);
        }
        catch (std::ios::failure&)
        {
            assert(ios.rdstate() == (std::ios::eofbit | std::ios::badbit));
        }
#endif
    }
    {
        testbuf sb;
        std::ios ios(&sb);
        ios.setstate(std::ios::goodbit);
        assert(ios.rdstate() == std::ios::goodbit);
        ios.setstate(std::ios::eofbit);
        assert(ios.rdstate() == std::ios::eofbit);
        ios.setstate(std::ios::failbit);
        assert(ios.rdstate() == (std::ios::eofbit | std::ios::failbit));
    }

  return;
}
