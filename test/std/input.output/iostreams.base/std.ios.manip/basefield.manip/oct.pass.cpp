// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostreams_base_std_ios_manip_basefield_manip_oct).
// main -> test_input_output_iostreams_base_std_ios_manip_basefield_manip_oct; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// class ios_base

// ios_base& oct(ios_base& str);

#include <ios>
#include <streambuf>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostreams_base_std_ios_manip_basefield_manip_oct { // libcis: isolate file-scope helpers
struct testbuf : public std::streambuf {};
} using namespace libcis_ns_input_output_iostreams_base_std_ios_manip_basefield_manip_oct; // libcis


void test_input_output_iostreams_base_std_ios_manip_basefield_manip_oct()
{
    testbuf sb;
    std::ios ios(&sb);
    std::ios_base& r = std::oct(ios);
    assert(&r == &ios);
    assert(ios.flags() & std::ios::oct);

  return;
}
