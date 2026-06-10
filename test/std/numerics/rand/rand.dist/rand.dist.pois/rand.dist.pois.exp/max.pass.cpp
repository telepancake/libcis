// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_exp_max).
// main -> test_numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_exp_max; file-scope helpers isolated in anon namespace.
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

// result_type max() const;

#include <random>

#include <cassert>
#include <limits>

#include "test_macros.h"

void test_numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_exp_max()
{
    {
        typedef std::exponential_distribution<> D;
        D d(.25);
        D::result_type m = d.max();
        assert(m == std::numeric_limits<D::result_type>::infinity());
    }

  return;
}
