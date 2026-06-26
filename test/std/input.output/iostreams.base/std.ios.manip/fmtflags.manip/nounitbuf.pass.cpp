// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_iostreams_base_std_ios_manip_fmtflags_manip_nounitbuf_3d8e856c).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// class ios_base

// ios_base& nounitbuf(ios_base& str);

#include <ios>
#include <streambuf>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostreams_base_std_ios_manip_fmtflags_manip_nounitbuf_3d8e856c { // libcis
struct testbuf : public std::streambuf {};

int main(int, char**)
{
    testbuf sb;
    std::ios ios(&sb);
    std::unitbuf(ios);
    std::ios_base& r = std::nounitbuf(ios);
    assert(&r == &ios);
    assert(!(ios.flags() & std::ios::unitbuf));

  return 0;

    return 0;
}
} // libcis_ns_input_output_iostreams_base_std_ios_manip_fmtflags_manip_nounitbuf_3d8e856c (libcis)

