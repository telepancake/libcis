// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_exp_eq).
// main -> test_numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_exp_eq; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class RealType = double>
// class exponential_distribution

// bool operator=(const exponential_distribution& x,
//                const exponential_distribution& y);
// bool operator!(const exponential_distribution& x,
//                const exponential_distribution& y);

#include <random>
#include <cassert>

#include "test_macros.h"

void test_numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_exp_eq()
{
    {
        typedef std::exponential_distribution<> D;
        D d1(.25);
        D d2(.25);
        assert(d1 == d2);
    }
    {
        typedef std::exponential_distribution<> D;
        D d1(.28);
        D d2(.25);
        assert(d1 != d2);
    }

  return;
}
