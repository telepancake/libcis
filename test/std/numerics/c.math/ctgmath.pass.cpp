// AST-transferred from libc++ by tools/transfer.py (slug=numerics_c_math_ctgmath).
// main -> test_numerics_c_math_ctgmath; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ctgmath> // deprecated in C++17, removed in C++20, but still provided by libc++ as an extension

// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS

#include <ctgmath>

#include "test_macros.h"

void test_numerics_c_math_ctgmath()
{
    std::complex<double> cd;
    (void)cd;
    double x = std::sin(0);
    ((void)x); // Prevent unused warning

  return;
}
