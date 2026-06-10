// AST-transferred from libc++ by tools/transfer.py (slug=language_support_support_types_offsetof).
// main -> test_language_support_support_types_offsetof; file-scope helpers isolated in anon namespace.
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

namespace libcis_ns_language_support_support_types_offsetof { // libcis: isolate file-scope helpers
struct A
{
    int x;
};
} using namespace libcis_ns_language_support_support_types_offsetof; // libcis


void test_language_support_support_types_offsetof()
{
    static_assert(noexcept(offsetof(A, x)), "");

  return;
}
