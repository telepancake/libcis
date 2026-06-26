// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_c_math_ctgmath_3ad8b249).
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

namespace libcis_ns_numerics_c_math_ctgmath_3ad8b249 { // libcis
int main(int, char**)
{
    std::complex<double> cd;
    (void)cd;
    double x = std::sin(0);
    ((void)x); // Prevent unused warning

  return 0;

    return 0;
}
} // libcis_ns_numerics_c_math_ctgmath_3ad8b249 (libcis)

