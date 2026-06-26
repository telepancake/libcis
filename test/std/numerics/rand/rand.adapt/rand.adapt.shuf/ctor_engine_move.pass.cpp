// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_rand_rand_adapt_rand_adapt_shuf_ctor_engine_move_f37775f9).
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
#include <utility>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_adapt_rand_adapt_shuf_ctor_engine_move_f37775f9 { // libcis
int main(int, char**)
{
    {
        typedef std::minstd_rand0 Engine;
        typedef std::knuth_b Adaptor;
        Engine e;
        Engine e0 = e;
        Adaptor a(std::move(e0));
        for (unsigned k = 0; k <= Adaptor::table_size; ++k) {
            (void)e();
        }

        assert(a.base() == e);
    }

  return 0;

    return 0;
}
} // libcis_ns_numerics_rand_rand_adapt_rand_adapt_shuf_ctor_engine_move_f37775f9 (libcis)

