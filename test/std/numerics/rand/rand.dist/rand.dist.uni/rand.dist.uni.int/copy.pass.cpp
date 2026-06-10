// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_uni_rand_dist_uni_int_copy).
// main -> test_numerics_rand_rand_dist_rand_dist_uni_rand_dist_uni_int_copy; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class _IntType = int>
// class uniform_int_distribution

// uniform_int_distribution(const uniform_int_distribution&);

#include <random>
#include <cassert>

#include "test_macros.h"

namespace { // libcis: isolate file-scope helpers
void
test1()
{
    typedef std::uniform_int_distribution<long> D;
    D d1(2, 5);
    D d2 = d1;
    assert(d1 == d2);
}
} // anonymous namespace (libcis)


void test_numerics_rand_rand_dist_rand_dist_uni_rand_dist_uni_int_copy()
{
    test1();

  return;
}
