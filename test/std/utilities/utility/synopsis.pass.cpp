// AST-transferred from libc++ by tools/transfer.py (slug=utilities_utility_synopsis).
// main -> test_utilities_utility_synopsis; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <utility>
// XFAIL: c++03

// #include <initializer_list>

#include <utility>

#include "test_macros.h"

void test_utilities_utility_synopsis()
{
    std::initializer_list<int> x;
    (void)x;

  return;
}
