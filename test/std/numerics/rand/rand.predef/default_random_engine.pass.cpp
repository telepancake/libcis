// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_predef_default_random_engine).
// main -> test_numerics_rand_rand_predef_default_random_engine; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// typedef minstd_rand0 default_random_engine;

#include <random>
#include <cassert>

#include "test_macros.h"

void test_numerics_rand_rand_predef_default_random_engine()
{
    std::default_random_engine e;
    e.discard(9999);
    LIBCPP_ASSERT(e() == 399268537u);

  return;
}
