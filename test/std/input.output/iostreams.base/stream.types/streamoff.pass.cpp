// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostreams_base_stream_types_streamoff).
// main -> test_input_output_iostreams_base_stream_types_streamoff; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// typedef OFF_T streamoff;

#include <ios>
#include <type_traits>

#include "test_macros.h"

void test_input_output_iostreams_base_stream_types_streamoff()
{
    static_assert(std::is_integral<std::streamoff>::value, "");
    static_assert(std::is_signed<std::streamoff>::value, "");

  return;
}
