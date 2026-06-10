// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostreams_base_fpos_fpos_operations_ctor_int).
// main -> test_input_output_iostreams_base_fpos_fpos_operations_ctor_int; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// template <class StateT> class fpos

// fpos(int)

#include <ios>
#include <cassert>

#include "test_macros.h"

void test_input_output_iostreams_base_fpos_fpos_operations_ctor_int()
{
    typedef std::fpos<std::mbstate_t> P;
    P p(5);
    assert(p == P(5));

  return;
}
