// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_adapt_rand_adapt_shuf_ctor_engine_copy).
// main -> test_numerics_rand_rand_adapt_rand_adapt_shuf_ctor_engine_copy; file-scope helpers isolated in anon namespace.
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

// explicit shuffle_order_engine(const Engine& e);

#include <random>
#include <cassert>

#include "test_macros.h"

void test_numerics_rand_rand_adapt_rand_adapt_shuf_ctor_engine_copy()
{
    {
        typedef std::minstd_rand0 Engine;
        typedef std::knuth_b Adaptor;
        Engine e;
        Adaptor a(e);
        for (unsigned k = 0; k <= Adaptor::table_size; ++k) {
            (void)e();
        }

        assert(a.base() == e);
    }

  return;
}
