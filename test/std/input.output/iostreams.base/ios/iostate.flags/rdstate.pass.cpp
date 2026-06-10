// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_iostate_flags_rdstate).
// main -> test_input_output_iostreams_base_ios_iostate_flags_rdstate; file-scope helpers isolated in anon namespace.
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

void test_input_output_iostreams_base_ios_iostate_flags_rdstate()
{
    std::ios ios(0);
    assert(ios.rdstate() == std::ios::badbit);
    ios.setstate(std::ios::failbit);
    assert(ios.rdstate() == (std::ios::failbit | std::ios::badbit));

  return;
}
