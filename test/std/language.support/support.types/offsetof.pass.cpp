// transferred+adapted from libc++ by tools/transfer.py (slug=language_support_support_types_offsetof_3124a437).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

#include <cstddef>

#include "test_macros.h"

#ifndef offsetof
#error offsetof not defined
#endif

namespace libcis_ns_language_support_support_types_offsetof_3124a437 { // libcis
struct A
{
    int x;
};

int main(int, char**)
{
    static_assert(noexcept(offsetof(A, x)), "");

  return 0;

    return 0;
}
} // libcis_ns_language_support_support_types_offsetof_3124a437 (libcis)

