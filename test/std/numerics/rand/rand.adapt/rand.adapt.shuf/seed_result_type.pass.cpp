// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_adapt_rand_adapt_shuf_seed_result_type).
// main -> test_numerics_rand_rand_adapt_rand_adapt_shuf_seed_result_type; file-scope helpers isolated in anon namespace.
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

// void seed(result_type s = default_seed);

#include <random>
#include <cassert>

#include "test_macros.h"

namespace { // libcis: isolate file-scope helpers
void
test1()
{
    for (int s = 0; s < 20; ++s)
    {
        typedef std::knuth_b E;
        E e1(s);
        E e2;
        e2.seed(s);
        assert(e1 == e2);
    }
}
} // anonymous namespace (libcis)


void test_numerics_rand_rand_adapt_rand_adapt_shuf_seed_result_type()
{
    test1();

  return;
}
