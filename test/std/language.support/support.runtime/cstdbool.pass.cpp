// AST-transferred from libc++ by tools/transfer.py (slug=language_support_support_runtime_cstdbool).
// main -> test_language_support_support_runtime_cstdbool; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// test <cstdbool> // deprecated in C++17, removed in C++20, but still provided by libc++ as an extension

// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS

#include <cstdbool>

#include "test_macros.h"

#ifndef __bool_true_false_are_defined
#error __bool_true_false_are_defined not defined
#endif

#ifdef bool
#error bool should not be defined
#endif

#ifdef true
#error true should not be defined
#endif

#ifdef false
#error false should not be defined
#endif

void test_language_support_support_runtime_cstdbool()
{

  return;
}
