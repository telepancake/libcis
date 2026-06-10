// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_bin_eq).
// main -> test_numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_bin_eq; file-scope helpers isolated in anon namespace.
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

// bool operator=(const binomial_distribution& x,
//                const binomial_distribution& y);
// bool operator!(const binomial_distribution& x,
//                const binomial_distribution& y);

#include <random>
#include <cassert>

#include "test_macros.h"

void test_numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_bin_eq()
{
    {
        typedef std::binomial_distribution<> D;
        D d1(3, .25);
        D d2(3, .25);
        assert(d1 == d2);
    }
    {
        typedef std::binomial_distribution<> D;
        D d1(3, .28);
        D d2(3, .25);
        assert(d1 != d2);
    }
    {
        typedef std::binomial_distribution<> D;
        D d1(3, .25);
        D d2(4, .25);
        assert(d1 != d2);
    }

  return;
}
