// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_base_fmtflags_state_width_streamsize_3f0b99d5).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// class ios_base

// streamsize width(streamsize wide);

#include <ios>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostreams_base_ios_base_fmtflags_state_width_streamsize_3f0b99d5 { // libcis
class test
    : public std::ios
{
public:
    test()
    {
        init(0);
    }
};

int main(int, char**)
{
    test t;
    assert(t.width() == 0);
    std::streamsize w = t.width(4);
    assert(w == 0);
    assert(t.width() == 4);

  return 0;

    return 0;
}
} // libcis_ns_input_output_iostreams_base_ios_base_fmtflags_state_width_streamsize_3f0b99d5 (libcis)

