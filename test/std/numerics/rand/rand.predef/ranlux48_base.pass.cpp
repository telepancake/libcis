// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_predef_ranlux48_base).
// main -> test_numerics_rand_rand_predef_ranlux48_base; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// typedef subtract_with_carry_engine<uint_fast64_t, 48,  5, 12>  ranlux48_base;

#include <random>
#include <cassert>

#include "test_macros.h"

void test_numerics_rand_rand_predef_ranlux48_base()
{
    std::ranlux48_base e;
    e.discard(9999);
    assert(e() == 61839128582725ull);

  return;
}
