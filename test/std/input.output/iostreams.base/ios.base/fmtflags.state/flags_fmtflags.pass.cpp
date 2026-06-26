// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_base_fmtflags_state_flags_fmtflags_0a04be4d).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// class ios_base

// fmtflags flags(fmtflags fmtfl);

#include <ios>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostreams_base_ios_base_fmtflags_state_flags_fmtflags_0a04be4d { // libcis
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
    assert(t.flags() == (test::skipws | test::dec));
    test::fmtflags f = t.flags(test::hex | test::right);
    assert(f == (test::skipws | test::dec));
    assert(t.flags() == (test::hex | test::right));

  return 0;

    return 0;
}
} // libcis_ns_input_output_iostreams_base_ios_base_fmtflags_state_flags_fmtflags_0a04be4d (libcis)

