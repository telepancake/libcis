// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_iostate_flags_good).
// main -> test_input_output_iostreams_base_ios_iostate_flags_good; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// template <class charT, class traits> class basic_ios

// bool good() const;

#include <ios>
#include <streambuf>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostreams_base_ios_iostate_flags_good { // libcis: isolate file-scope helpers
struct testbuf : public std::streambuf {};
} using namespace libcis_ns_input_output_iostreams_base_ios_iostate_flags_good; // libcis


void test_input_output_iostreams_base_ios_iostate_flags_good()
{
    {
        std::ios ios(0);
        assert(!ios.good());
    }
    {
        testbuf sb;
        std::ios ios(&sb);
        assert(ios.good());
        ios.setstate(std::ios::eofbit);
        assert(!ios.good());
    }

  return;
}
