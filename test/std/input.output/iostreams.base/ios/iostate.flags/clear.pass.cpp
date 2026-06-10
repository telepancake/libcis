// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_iostate_flags_clear).
// main -> test_input_output_iostreams_base_ios_iostate_flags_clear; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// template <class charT, class traits> class basic_ios

// void clear(iostate state = goodbit);

#include <ios>
#include <streambuf>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostreams_base_ios_iostate_flags_clear { // libcis: isolate file-scope helpers
struct testbuf : public std::streambuf {};
} using namespace libcis_ns_input_output_iostreams_base_ios_iostate_flags_clear; // libcis


void test_input_output_iostreams_base_ios_iostate_flags_clear()
{
    {
        std::ios ios(0);
        ios.clear();
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
            ios.clear();
            assert(false);
        }
        catch (std::ios::failure&)
        {
            assert(ios.rdstate() == std::ios::badbit);
        }
        try
        {
            ios.clear(std::ios::eofbit);
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
        ios.clear();
        assert(ios.rdstate() == std::ios::goodbit);
        ios.exceptions(std::ios::badbit);
        ios.clear();
        assert(ios.rdstate() == std::ios::goodbit);
        ios.clear(std::ios::eofbit);
        assert(ios.rdstate() == std::ios::eofbit);
    }

  return;
}
