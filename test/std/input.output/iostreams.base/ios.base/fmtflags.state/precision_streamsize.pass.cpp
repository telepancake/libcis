// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_base_fmtflags_state_precision_streamsize).
// main -> test_input_output_iostreams_base_ios_base_fmtflags_state_precision_streamsize; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// class ios_base

// streamsize precision(streamsize prec);

#include <ios>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostreams_base_ios_base_fmtflags_state_precision_streamsize { // libcis: isolate file-scope helpers
class test
    : public std::ios
{
public:
    test()
    {
        init(0);
    }
};
} using namespace libcis_ns_input_output_iostreams_base_ios_base_fmtflags_state_precision_streamsize; // libcis


void test_input_output_iostreams_base_ios_base_fmtflags_state_precision_streamsize()
{
    test t;
    assert(t.precision() == 6);
    std::streamsize p = t.precision(10);
    assert(p == 6);
    assert(t.precision() == 10);

  return;
}
