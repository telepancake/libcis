// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_samp_rand_dist_samp_discrete_assign).
// main -> test_numerics_rand_rand_dist_rand_dist_samp_rand_dist_samp_discrete_assign; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class IntType = int>
// class discrete_distribution

// discrete_distribution& operator=(const discrete_distribution&);

#include <random>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_dist_rand_dist_samp_rand_dist_samp_discrete_assign { // libcis: isolate file-scope helpers
void
test1()
{
    typedef std::discrete_distribution<> D;
    double p[] = {2, 4, 1, 8};
    D d1(p, p+4);
    D d2;
    assert(d1 != d2);
    d2 = d1;
    assert(d1 == d2);
}
} using namespace libcis_ns_numerics_rand_rand_dist_rand_dist_samp_rand_dist_samp_discrete_assign; // libcis


void test_numerics_rand_rand_dist_rand_dist_samp_rand_dist_samp_discrete_assign()
{
    test1();

  return;
}
