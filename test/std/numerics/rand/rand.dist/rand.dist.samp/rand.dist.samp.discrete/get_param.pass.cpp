// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_samp_rand_dist_samp_discrete_get_param).
// main -> test_numerics_rand_rand_dist_rand_dist_samp_rand_dist_samp_discrete_get_param; file-scope helpers isolated in anon namespace.
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

// param_type param() const;

#include <random>
#include <cassert>

#include "test_macros.h"

void test_numerics_rand_rand_dist_rand_dist_samp_rand_dist_samp_discrete_get_param()
{
    {
        typedef std::discrete_distribution<> D;
        typedef D::param_type P;
        double p0[] = {.3, .1, .6};
        P p(p0, p0+3);
        D d(p);
        assert(d.param() == p);
    }

  return;
}
