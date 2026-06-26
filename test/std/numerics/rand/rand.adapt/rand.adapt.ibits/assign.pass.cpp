// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_rand_rand_adapt_rand_adapt_ibits_assign_c39d152d).
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

// independent_bits_engine& operator=(const independent_bits_engine&);

#include <random>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_adapt_rand_adapt_ibits_assign_c39d152d { // libcis
void
test1()
{
    typedef std::independent_bits_engine<std::ranlux24, 32, unsigned> E;
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
    typedef std::independent_bits_engine<std::ranlux48, 64, unsigned long long> E;
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

int main(int, char**)
{
    test1();
    test2();

  return 0;

    return 0;
}
} // libcis_ns_numerics_rand_rand_adapt_rand_adapt_ibits_assign_c39d152d (libcis)

