// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_bin_assign_b55c4141).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class IntType = int>
// class binomial_distribution

// binomial_distribution& operator=(const binomial_distribution&);

#include <random>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_bin_assign_b55c4141 { // libcis
void
test1()
{
    typedef std::binomial_distribution<> D;
    D d1(2, 0.75);
    D d2;
    assert(d1 != d2);
    d2 = d1;
    assert(d1 == d2);
}

int main(int, char**)
{
    test1();

  return 0;

    return 0;
}
} // libcis_ns_numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_bin_assign_b55c4141 (libcis)

