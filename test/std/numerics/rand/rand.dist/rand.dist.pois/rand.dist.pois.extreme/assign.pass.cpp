// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_extreme_assign).
// main -> test_numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_extreme_assign; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class RealType = double>
// class extreme_value_distribution

// extreme_value_distribution& operator=(const extreme_value_distribution&);

#include <random>
#include <cassert>

#include "test_macros.h"

namespace { // libcis: isolate file-scope helpers
void
test1()
{
    typedef std::extreme_value_distribution<> D;
    D d1(.5, 2);
    D d2;
    assert(d1 != d2);
    d2 = d1;
    assert(d1 == d2);
}
} // anonymous namespace (libcis)


void test_numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_extreme_assign()
{
    test1();

  return;
}
