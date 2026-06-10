// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_iostate_flags_bad).
// main -> test_input_output_iostreams_base_ios_iostate_flags_bad; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// template <class charT, class traits> class basic_ios

// bool bad() const;

#include <ios>
#include <streambuf>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostreams_base_ios_iostate_flags_bad { // libcis: isolate file-scope helpers
struct testbuf : public std::streambuf {};
} using namespace libcis_ns_input_output_iostreams_base_ios_iostate_flags_bad; // libcis


void test_input_output_iostreams_base_ios_iostate_flags_bad()
{
    {
        std::ios ios(0);
        assert(ios.bad());
        ios.setstate(std::ios::eofbit);
        assert(ios.bad());
    }
    {
        testbuf sb;
        std::ios ios(&sb);
        assert(!ios.bad());
        ios.setstate(std::ios::eofbit);
        assert(!ios.bad());
        ios.setstate(std::ios::failbit);
        assert(!ios.bad());
        ios.setstate(std::ios::badbit);
        assert(ios.bad());
    }

  return;
}
