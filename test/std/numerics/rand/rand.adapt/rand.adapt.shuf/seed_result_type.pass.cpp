// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_rand_rand_adapt_rand_adapt_shuf_seed_result_type_4c8fe98f).
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

namespace libcis_ns_numerics_rand_rand_adapt_rand_adapt_shuf_seed_result_type_4c8fe98f { // libcis
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

int main(int, char**)
{
    test1();

  return 0;

    return 0;
}
} // libcis_ns_numerics_rand_rand_adapt_rand_adapt_shuf_seed_result_type_4c8fe98f (libcis)

