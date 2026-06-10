// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_iostate_flags_exceptions).
// main -> test_input_output_iostreams_base_ios_iostate_flags_exceptions; file-scope helpers isolated in anon namespace.
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

namespace libcis_ns_input_output_iostreams_base_ios_iostate_flags_exceptions { // libcis: isolate file-scope helpers
struct testbuf : public std::streambuf {};
} using namespace libcis_ns_input_output_iostreams_base_ios_iostate_flags_exceptions; // libcis


void test_input_output_iostreams_base_ios_iostate_flags_exceptions()
{
    {
        const std::ios ios(0);
        assert(ios.exceptions() == std::ios::goodbit);
    }
    {
        testbuf sb;
        const std::ios ios(&sb);
        assert(ios.exceptions() == std::ios::goodbit);
    }

  return;
}
