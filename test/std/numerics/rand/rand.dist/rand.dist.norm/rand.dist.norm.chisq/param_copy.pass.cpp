// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_norm_rand_dist_norm_chisq_param_copy).
// main -> test_numerics_rand_rand_dist_rand_dist_norm_rand_dist_norm_chisq_param_copy; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class RealType = double>
// class chi_squared_distribution
// {
//     class param_type;

#include <random>
#include <limits>
#include <cassert>

#include "test_macros.h"

void test_numerics_rand_rand_dist_rand_dist_norm_rand_dist_norm_chisq_param_copy()
{
    {
        typedef std::chi_squared_distribution<> D;
        typedef D::param_type param_type;
        param_type p0(10);
        param_type p = p0;
        assert(p.n() == 10);
    }

  return;
}
