// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_base_ios_types_ios_seekdir_seekdir).
// main -> test_input_output_iostreams_base_ios_base_ios_types_ios_seekdir_seekdir; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// class ios_base

// static const seekdir beg;
// static const seekdir cur;
// static const seekdir end;

#include <ios>
#include <cassert>

#include "test_macros.h"

void test_input_output_iostreams_base_ios_base_ios_types_ios_seekdir_seekdir()
{
    assert(std::ios_base::beg != std::ios_base::cur);
    assert(std::ios_base::beg != std::ios_base::end);
    assert(std::ios_base::cur != std::ios_base::end);

  return;
}
