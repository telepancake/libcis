// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_iostate_flags_rdstate_22a97e2d).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// template <class charT, class traits> class basic_ios

// iostate rdstate() const;

#include <ios>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostreams_base_ios_iostate_flags_rdstate_22a97e2d { // libcis
int main(int, char**)
{
    std::ios ios(0);
    assert(ios.rdstate() == std::ios::badbit);
    ios.setstate(std::ios::failbit);
    assert(ios.rdstate() == (std::ios::failbit | std::ios::badbit));

  return 0;

    return 0;
}
} // libcis_ns_input_output_iostreams_base_ios_iostate_flags_rdstate_22a97e2d (libcis)

