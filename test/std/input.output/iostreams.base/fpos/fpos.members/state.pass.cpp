// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_iostreams_base_fpos_fpos_members_state_aaabcc83).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// template <class StateT> class fpos

// void state(stateT s);

#include <ios>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostreams_base_fpos_fpos_members_state_aaabcc83 { // libcis
int main(int, char**)
{
    std::fpos<int> f;
    f.state(3);
    assert(f.state() == 3);

  return 0;

    return 0;
}
} // libcis_ns_input_output_iostreams_base_fpos_fpos_members_state_aaabcc83 (libcis)

