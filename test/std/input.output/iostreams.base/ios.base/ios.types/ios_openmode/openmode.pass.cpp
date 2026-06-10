// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_base_ios_types_ios_openmode_openmode).
// main -> test_input_output_iostreams_base_ios_base_ios_types_ios_openmode_openmode; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// class ios_base

// static const openmode app;
// static const openmode ate;
// static const openmode binary;
// static const openmode in;
// static const openmode out;
// static const openmode trunc;

#include <ios>
#include <cassert>

#include "test_macros.h"

void test_input_output_iostreams_base_ios_base_ios_types_ios_openmode_openmode()
{
    assert(std::ios_base::app);
    assert(std::ios_base::ate);
    assert(std::ios_base::binary);
    assert(std::ios_base::in);
    assert(std::ios_base::out);
    assert(std::ios_base::trunc);

    assert
    (
        ( std::ios_base::app
        & std::ios_base::ate
        & std::ios_base::binary
        & std::ios_base::in
        & std::ios_base::out
        & std::ios_base::trunc) == 0
    );

  return;
}
