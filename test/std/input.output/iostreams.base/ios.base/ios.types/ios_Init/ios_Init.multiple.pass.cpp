// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_base_ios_types_ios_Init_ios_Init_multiple).
// main -> test_input_output_iostreams_base_ios_base_ios_types_ios_Init_ios_Init_multiple; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <iostream>
#include <cassert>

#include "test_macros.h"

// Test to make sure that the streams only get initialized once
// Taken from https://llvm.org/PR43300

void test_input_output_iostreams_base_ios_base_ios_types_ios_Init_ios_Init_multiple()
{

    std::cout << "Hello!";
    std::ios_base::fmtflags stock_flags = std::cout.flags();

    std::cout << std::boolalpha << true;
    std::ios_base::fmtflags ba_flags = std::cout.flags();
    assert(stock_flags != ba_flags);

    std::ios_base::Init init_streams;
    std::ios_base::fmtflags after_init = std::cout.flags();
    assert(after_init == ba_flags);

    return;
}
