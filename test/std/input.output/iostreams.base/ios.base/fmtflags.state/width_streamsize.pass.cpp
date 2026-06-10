// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_base_fmtflags_state_width_streamsize).
// main -> test_input_output_iostreams_base_ios_base_fmtflags_state_width_streamsize; file-scope helpers isolated in anon namespace.
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

namespace libcis_ns_input_output_iostreams_base_ios_base_fmtflags_state_width_streamsize { // libcis: isolate file-scope helpers
class test
    : public std::ios
{
public:
    test()
    {
        init(0);
    }
};
} using namespace libcis_ns_input_output_iostreams_base_ios_base_fmtflags_state_width_streamsize; // libcis


void test_input_output_iostreams_base_ios_base_fmtflags_state_width_streamsize()
{
    test t;
    assert(t.width() == 0);
    std::streamsize w = t.width(4);
    assert(w == 0);
    assert(t.width() == 4);

  return;
}
