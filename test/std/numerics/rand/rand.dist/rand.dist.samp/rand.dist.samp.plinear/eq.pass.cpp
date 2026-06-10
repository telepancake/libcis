// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_samp_rand_dist_samp_plinear_eq).
// main -> test_numerics_rand_rand_dist_rand_dist_samp_rand_dist_samp_plinear_eq; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class RealType = double>
// class piecewise_linear_distribution

// bool operator=(const piecewise_linear_distribution& x,
//                const piecewise_linear_distribution& y);
// bool operator!(const piecewise_linear_distribution& x,
//                const piecewise_linear_distribution& y);

#include <random>
#include <cassert>

#include "test_macros.h"

void test_numerics_rand_rand_dist_rand_dist_samp_rand_dist_samp_plinear_eq()
{
    {
        typedef std::piecewise_linear_distribution<> D;
        D d1;
        D d2;
        assert(d1 == d2);
    }
    {
        typedef std::piecewise_linear_distribution<> D;
        double b[] = {10, 14, 16, 17};
        double p[] = {25, 62.5, 12.5, 1};
        D d1(b, b+4, p);
        D d2(b, b+4, p);
        assert(d1 == d2);
    }
    {
        typedef std::piecewise_linear_distribution<> D;
        double b[] = {10, 14, 16, 17};
        double p[] = {25, 62.5, 12.5, 0};
        D d1(b, b+4, p);
        D d2;
        assert(d1 != d2);
    }

  return;
}
