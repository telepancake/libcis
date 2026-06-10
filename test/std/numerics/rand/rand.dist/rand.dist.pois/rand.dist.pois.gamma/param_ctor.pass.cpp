// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_gamma_param_ctor).
// main -> test_numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_gamma_param_ctor; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class RealType = double>
// class gamma_distribution
// {
//     class param_type;

#include <random>
#include <limits>
#include <cassert>

#include "test_macros.h"

void test_numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_gamma_param_ctor()
{
    {
        typedef std::gamma_distribution<> D;
        typedef D::param_type param_type;
        param_type p;
        assert(p.alpha() == 1);
        assert(p.beta() == 1);
    }
    {
        typedef std::gamma_distribution<> D;
        typedef D::param_type param_type;
        param_type p(10);
        assert(p.alpha() == 10);
        assert(p.beta() == 1);
    }
    {
        typedef std::gamma_distribution<> D;
        typedef D::param_type param_type;
        param_type p(10, 5);
        assert(p.alpha() == 10);
        assert(p.beta() == 5);
    }

  return;
}
