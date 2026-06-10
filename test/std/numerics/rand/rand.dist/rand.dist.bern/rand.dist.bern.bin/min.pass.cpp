// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_bin_min).
// main -> test_numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_bin_min; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// Missing lgamma_r
// XFAIL: LLVM-LIBC-FIXME

// <random>

// template<class IntType = int>
// class binomial_distribution

// result_type min() const;

#include <random>
#include <cassert>

#include "test_macros.h"

void test_numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_bin_min()
{
    {
        typedef std::binomial_distribution<> D;
        D d(4, .5);
        assert(d.min() == 0);
    }

  return;
}
