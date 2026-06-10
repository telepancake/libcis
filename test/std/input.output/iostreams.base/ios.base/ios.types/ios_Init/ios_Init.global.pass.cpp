// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_base_ios_types_ios_Init_ios_Init_global).
// main -> test_input_output_iostreams_base_ios_base_ios_types_ios_Init_ios_Init_global; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <iostream>

// FIXME: Remove after issue https://llvm.org/PR127348 resolved.
extern "C" const char* __asan_default_options() { return "check_initialization_order=true:strict_init_order=true"; }

// Test that ios used from globals constructors doesn't trigger Asan initialization-order-fiasco.

namespace libcis_ns_input_output_iostreams_base_ios_base_ios_types_ios_Init_ios_Init_global { // libcis: isolate file-scope helpers
struct Global {
  Global() { std::cout << "Hello!"; }
} global;
} using namespace libcis_ns_input_output_iostreams_base_ios_base_ios_types_ios_Init_ios_Init_global; // libcis


void test_input_output_iostreams_base_ios_base_ios_types_ios_Init_ios_Init_global() { return; }
