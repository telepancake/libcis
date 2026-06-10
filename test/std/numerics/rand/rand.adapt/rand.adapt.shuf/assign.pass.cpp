// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_adapt_rand_adapt_shuf_assign).
// main -> test_numerics_rand_rand_adapt_rand_adapt_shuf_assign; file-scope helpers isolated in anon namespace.
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

// shuffle_order_engine& operator=(const shuffle_order_engine&);

#include <random>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_adapt_rand_adapt_shuf_assign { // libcis: isolate file-scope helpers
void
test1()
{
    typedef std::knuth_b E;
    E e1(2);
    (void)e1();
    E e2(5);
    e2 = e1;
    assert(e1 == e2);
    assert(e1() == e2());
    E::result_type k = e1();
    assert(e1 != e2);
    assert(e2() == k);
    assert(e1 == e2);
}
} using namespace libcis_ns_numerics_rand_rand_adapt_rand_adapt_shuf_assign; // libcis


void test_numerics_rand_rand_adapt_rand_adapt_shuf_assign()
{
    test1();

  return;
}
