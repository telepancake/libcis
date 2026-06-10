// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_poisson_min).
// main -> test_numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_poisson_min; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class IntType = int>
// class poisson_distribution

// result_type min() const;

#include <random>
#include <cassert>

#include "test_macros.h"

void test_numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_poisson_min()
{
    {
        typedef std::poisson_distribution<> D;
        D d(.5);
        assert(d.min() == 0);
    }

  return;
}
