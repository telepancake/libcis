// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_base_fmtflags_state_precision_39f44b94).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// class ios_base

// streamsize precision() const;

#include <ios>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostreams_base_ios_base_fmtflags_state_precision_39f44b94 { // libcis
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
    const test t;
    assert(t.precision() == 6);

  return 0;

    return 0;
}
} // libcis_ns_input_output_iostreams_base_ios_base_fmtflags_state_precision_39f44b94 (libcis)

