// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_iostreams_base_std_ios_manip_fmtflags_manip_noboolalpha_2fd96ce7).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// class ios_base

// ios_base& noboolalpha(ios_base& str);

#include <ios>
#include <streambuf>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostreams_base_std_ios_manip_fmtflags_manip_noboolalpha_2fd96ce7 { // libcis
struct testbuf : public std::streambuf {};

int main(int, char**)
{
    testbuf sb;
    std::ios ios(&sb);
    std::boolalpha(ios);
    std::ios_base& r = std::noboolalpha(ios);
    assert(&r == &ios);
    assert(!(ios.flags() & std::ios::boolalpha));

  return 0;

    return 0;
}
} // libcis_ns_input_output_iostreams_base_std_ios_manip_fmtflags_manip_noboolalpha_2fd96ce7 (libcis)

