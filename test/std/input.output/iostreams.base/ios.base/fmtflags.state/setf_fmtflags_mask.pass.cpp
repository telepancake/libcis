// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_base_fmtflags_state_setf_fmtflags_mask).
// main -> test_input_output_iostreams_base_ios_base_fmtflags_state_setf_fmtflags_mask; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// class ios_base

// fmtflags setf(fmtflags fmtfl, fmtflags mask);

#include <ios>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostreams_base_ios_base_fmtflags_state_setf_fmtflags_mask { // libcis: isolate file-scope helpers
class test
    : public std::ios
{
public:
    test()
    {
        init(0);
    }
};
} using namespace libcis_ns_input_output_iostreams_base_ios_base_fmtflags_state_setf_fmtflags_mask; // libcis


void test_input_output_iostreams_base_ios_base_fmtflags_state_setf_fmtflags_mask()
{
    test t;
    assert(t.flags() == (test::skipws | test::dec));
    test::fmtflags f = t.setf(test::hex | test::right, test::dec | test::right);
    assert(f == (test::skipws | test::dec));
    assert(t.flags() == (test::skipws | test::right));

  return;
}
