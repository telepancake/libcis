// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_base_ios_base_storage_xalloc).
// main -> test_input_output_iostreams_base_ios_base_ios_base_storage_xalloc; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// class ios_base

// static int xalloc();

#include <ios>
#include <cassert>

#include "test_macros.h"

void test_input_output_iostreams_base_ios_base_ios_base_storage_xalloc()
{
    int index = std::ios_base::xalloc();
    for (int i = 0; i < 10000; ++i)
        assert(std::ios_base::xalloc() == ++index);

  return;
}
