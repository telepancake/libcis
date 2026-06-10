// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_bin_ctor_param).
// main -> test_numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_bin_ctor_param; file-scope helpers isolated in anon namespace.
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

// explicit binomial_distribution(const param_type& parm);

#include <random>
#include <cassert>

#include "test_macros.h"

void test_numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_bin_ctor_param()
{
    {
        typedef std::binomial_distribution<> D;
        typedef D::param_type P;
        P p(5, 0.25);
        D d(p);
        assert(d.t() == 5);
        assert(d.p() == 0.25);
    }

  return;
}
