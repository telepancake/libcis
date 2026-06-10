// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_adapt_rand_adapt_shuf_default).
// main -> test_numerics_rand_rand_adapt_rand_adapt_shuf_default; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class Engine, size_t k>
// class shuffle_order_engine

// explicit shuffle_order_engine();

#include <random>
#include <cassert>

#include "test_macros.h"

namespace { // libcis: isolate file-scope helpers
void
test1()
{
    std::knuth_b e1;
    std::knuth_b e2(std::minstd_rand0::default_seed);
    assert(e1 == e2);
    assert(e1() == 152607844u);
}
} // anonymous namespace (libcis)


void test_numerics_rand_rand_adapt_rand_adapt_shuf_default()
{
    test1();

  return;
}
