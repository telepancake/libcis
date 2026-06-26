// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_iostreams_base_fpos_fpos_operations_difference_810d366a).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// template <class StateT> class fpos

// Subtraction with fpos

#include <ios>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostreams_base_fpos_fpos_operations_difference_810d366a { // libcis
int main(int, char**)
{
    typedef std::fpos<std::mbstate_t> P;
    P p(11);
    P q(6);
    std::streamoff o = p - q;
    assert(o == 5);

  return 0;

    return 0;
}
} // libcis_ns_input_output_iostreams_base_fpos_fpos_operations_difference_810d366a (libcis)

