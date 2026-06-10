// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_negbin_param_copy).
// main -> test_numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_negbin_param_copy; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class IntType = int>
// class negative_binomial_distribution
// {
//     class param_type;

#include <random>
#include <limits>
#include <cassert>

#include "test_macros.h"

void test_numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_negbin_param_copy()
{
    {
        typedef std::negative_binomial_distribution<> D;
        typedef D::param_type param_type;
        param_type p0(10, .125);
        param_type p = p0;
        assert(p.k() == 10);
        assert(p.p() == .125);
    }

  return;
}
