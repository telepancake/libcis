// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_geo_eq).
// main -> test_numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_geo_eq; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class IntType = int>
// class geometric_distribution

// bool operator=(const geometric_distribution& x,
//                const geometric_distribution& y);
// bool operator!(const geometric_distribution& x,
//                const geometric_distribution& y);

#include <random>
#include <cassert>

#include "test_macros.h"

void test_numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_geo_eq()
{
    {
        typedef std::geometric_distribution<> D;
        D d1(.25);
        D d2(.25);
        assert(d1 == d2);
    }
    {
        typedef std::geometric_distribution<> D;
        D d1(.28);
        D d2(.25);
        assert(d1 != d2);
    }

  return;
}
