// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_base_fmtflags_state_precision_streamsize_cb0e007d).
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

namespace libcis_ns_input_output_iostreams_base_ios_base_fmtflags_state_precision_streamsize_cb0e007d { // libcis
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
    assert(t.precision() == 6);
    std::streamsize p = t.precision(10);
    assert(p == 6);
    assert(t.precision() == 10);

  return 0;

    return 0;
}
} // libcis_ns_input_output_iostreams_base_ios_base_fmtflags_state_precision_streamsize_cb0e007d (libcis)

