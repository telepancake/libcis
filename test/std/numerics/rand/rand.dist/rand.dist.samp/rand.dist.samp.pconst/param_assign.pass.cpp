// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_samp_rand_dist_samp_pconst_param_assign).
// main -> test_numerics_rand_rand_dist_rand_dist_samp_rand_dist_samp_pconst_param_assign; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class RealType = double>
// class piecewise_constant_distribution
// {
//     class param_type;

#include <random>
#include <limits>
#include <cassert>

#include "test_macros.h"

void test_numerics_rand_rand_dist_rand_dist_samp_rand_dist_samp_pconst_param_assign()
{
    {
        typedef std::piecewise_constant_distribution<> D;
        typedef D::param_type P;
        double b[] = {10, 14, 16, 17};
        double p[] = {25, 62.5, 12.5};
        const std::size_t Np = sizeof(p) / sizeof(p[0]);
        P p0(b, b+Np+1, p);
        P p1;
        p1 = p0;
        assert(p1 == p0);
    }

  return;
}
