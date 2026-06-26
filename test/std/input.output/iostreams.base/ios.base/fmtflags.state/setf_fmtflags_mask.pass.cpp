// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_base_fmtflags_state_setf_fmtflags_mask_b35523fd).
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

namespace libcis_ns_input_output_iostreams_base_ios_base_fmtflags_state_setf_fmtflags_mask_b35523fd { // libcis
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
    test::fmtflags f = t.setf(test::hex | test::right, test::dec | test::right);
    assert(f == (test::skipws | test::dec));
    assert(t.flags() == (test::skipws | test::right));

  return 0;

    return 0;
}
} // libcis_ns_input_output_iostreams_base_ios_base_fmtflags_state_setf_fmtflags_mask_b35523fd (libcis)

