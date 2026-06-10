// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_base_fmtflags_state_flags).
// main -> test_input_output_iostreams_base_ios_base_fmtflags_state_flags; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// class ios_base

// fmtflags flags() const;

#include <ios>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostreams_base_ios_base_fmtflags_state_flags { // libcis: isolate file-scope helpers
class test
    : public std::ios
{
public:
    test()
    {
        init(0);
    }
};
} using namespace libcis_ns_input_output_iostreams_base_ios_base_fmtflags_state_flags; // libcis


void test_input_output_iostreams_base_ios_base_fmtflags_state_flags()
{
    const test t;
    assert(t.flags() == (test::skipws | test::dec));

  return;
}
