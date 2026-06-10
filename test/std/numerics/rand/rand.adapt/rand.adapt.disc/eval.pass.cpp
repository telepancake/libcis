// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_adapt_rand_adapt_disc_eval).
// main -> test_numerics_rand_rand_adapt_rand_adapt_disc_eval; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class Engine, size_t p, size_t r>
// class discard_block_engine

// result_type operator()();

#include <random>
#include <cassert>

#include "test_macros.h"

namespace { // libcis: isolate file-scope helpers
void
test1()
{
    std::ranlux24 e;
    assert(e() == 15039276u);
    assert(e() == 16323925u);
    assert(e() == 14283486u);
}

void
test2()
{
    std::ranlux48 e;
    assert(e() == 23459059301164ull);
    assert(e() == 28639057539807ull);
    assert(e() == 276846226770426ull);
}
} // anonymous namespace (libcis)


void test_numerics_rand_rand_adapt_rand_adapt_disc_eval()
{
    test1();
    test2();

  return;
}
