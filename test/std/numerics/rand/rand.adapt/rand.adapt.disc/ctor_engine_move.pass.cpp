// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_adapt_rand_adapt_disc_ctor_engine_move).
// main -> test_numerics_rand_rand_adapt_rand_adapt_disc_ctor_engine_move; file-scope helpers isolated in anon namespace.
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

// explicit discard_block_engine(const Engine& e);

#include <random>
#include <cassert>
#include <utility>

#include "test_macros.h"

void test_numerics_rand_rand_adapt_rand_adapt_disc_ctor_engine_move()
{
    {
        typedef std::ranlux24_base Engine;
        typedef std::ranlux24 Adaptor;
        Engine e;
        Engine e0 = e;
        Adaptor a(std::move(e0));
        assert(a.base() == e);
    }

  return;
}
