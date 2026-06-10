// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_predef_ranlux48).
// main -> test_numerics_rand_rand_predef_ranlux48; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// typedef discard_block_engine<ranlux48_base, 389, 11>                ranlux48;

#include <random>
#include <cassert>

#include "test_macros.h"

void test_numerics_rand_rand_predef_ranlux48()
{
    std::ranlux48 e;
    e.discard(9999);
    assert(e() == 249142670248501ull);

  return;
}
