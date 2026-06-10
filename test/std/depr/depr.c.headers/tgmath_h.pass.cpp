// AST-transferred from libc++ by tools/transfer.py (slug=depr_depr_c_headers_tgmath_h).
// main -> test_depr_depr_c_headers_tgmath_h; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <tgmath.h>

#include <tgmath.h>

void test_depr_depr_c_headers_tgmath_h() {
    std::complex<double> cd;
    (void)cd;
    double x = sin(1.0);
    (void)x; // to placate scan-build

    return;
}
