// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_iostreams_base_fpos_fpos_operations_addition_f524bab7).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// template <class StateT> class fpos

// Addition

#include <ios>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostreams_base_fpos_fpos_operations_addition_f524bab7 { // libcis
int main(int, char**)
{
    typedef std::fpos<std::mbstate_t> P;
    P p(5);
    std::streamoff o(6);
    P q = p + o;
    assert(q == P(11));
    p += o;
    assert(p == q);

  return 0;

    return 0;
}
} // libcis_ns_input_output_iostreams_base_fpos_fpos_operations_addition_f524bab7 (libcis)

