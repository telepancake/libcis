// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_norm_rand_dist_norm_chisq_set_param).
// main -> test_numerics_rand_rand_dist_rand_dist_norm_rand_dist_norm_chisq_set_param; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class RealType = double>
// class chi_squared_distribution;

// void param(const param_type& parm);

#include <random>
#include <cassert>

#include "test_macros.h"

void test_numerics_rand_rand_dist_rand_dist_norm_rand_dist_norm_chisq_set_param()
{
    {
        typedef std::chi_squared_distribution<> D;
        typedef D::param_type P;
        P p(0.25);
        D d(0.75);
        d.param(p);
        assert(d.param() == p);
    }

  return;
}
