// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_norm_rand_dist_norm_chisq_assign).
// main -> test_numerics_rand_rand_dist_rand_dist_norm_rand_dist_norm_chisq_assign; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class RealType = double>
// class chi_squared_distribution

// chi_squared_distribution& operator=(const chi_squared_distribution&);

#include <random>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_dist_rand_dist_norm_rand_dist_norm_chisq_assign { // libcis: isolate file-scope helpers
void
test1()
{
    typedef std::chi_squared_distribution<> D;
    D d1(20.75);
    D d2;
    assert(d1 != d2);
    d2 = d1;
    assert(d1 == d2);
}
} using namespace libcis_ns_numerics_rand_rand_dist_rand_dist_norm_rand_dist_norm_chisq_assign; // libcis


void test_numerics_rand_rand_dist_rand_dist_norm_rand_dist_norm_chisq_assign()
{
    test1();

  return;
}
