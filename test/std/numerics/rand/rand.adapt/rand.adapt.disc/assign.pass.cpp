// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_adapt_rand_adapt_disc_assign).
// main -> test_numerics_rand_rand_adapt_rand_adapt_disc_assign; file-scope helpers isolated in anon namespace.
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

// discard_block_engine& operator=(const discard_block_engine&);

#include <random>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_adapt_rand_adapt_disc_assign { // libcis: isolate file-scope helpers
void
test1()
{
    typedef std::ranlux24 E;
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

void
test2()
{
    typedef std::ranlux48 E;
    E e1(3);
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
} using namespace libcis_ns_numerics_rand_rand_adapt_rand_adapt_disc_assign; // libcis


void test_numerics_rand_rand_adapt_rand_adapt_disc_assign()
{
    test1();
    test2();

  return;
}
