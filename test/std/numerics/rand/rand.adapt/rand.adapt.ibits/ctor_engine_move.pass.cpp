// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_adapt_rand_adapt_ibits_ctor_engine_move).
// main -> test_numerics_rand_rand_adapt_rand_adapt_ibits_ctor_engine_move; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class Engine, size_t w, class UIntType>
// class independent_bits_engine

// explicit independent_bits_engine(const Engine& e);

#include <random>

#include <cassert>
#include <utility>

#include "test_macros.h"

void test_numerics_rand_rand_adapt_rand_adapt_ibits_ctor_engine_move()
{
    {
        typedef std::mt19937 Engine;
        typedef std::independent_bits_engine<Engine, 24, unsigned> Adaptor;
        Engine e;
        Engine e0 = e;
        Adaptor a(std::move(e0));
        assert(a.base() == e);
    }

  return;
}
