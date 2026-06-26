// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_norm_rand_dist_norm_lognormal_param_ctor_45f379d7).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class RealType = double>
// class lognormal_distribution
// {
//     class param_type;

#include <random>
#include <limits>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_dist_rand_dist_norm_rand_dist_norm_lognormal_param_ctor_45f379d7 { // libcis
int main(int, char**)
{
    {
        typedef std::lognormal_distribution<> D;
        typedef D::param_type param_type;
        param_type p;
        assert(p.m() == 0);
        assert(p.s() == 1);
    }
    {
        typedef std::lognormal_distribution<> D;
        typedef D::param_type param_type;
        param_type p(10);
        assert(p.m() == 10);
        assert(p.s() == 1);
    }
    {
        typedef std::lognormal_distribution<> D;
        typedef D::param_type param_type;
        param_type p(10, 5);
        assert(p.m() == 10);
        assert(p.s() == 5);
    }

  return 0;

    return 0;
}
} // libcis_ns_numerics_rand_rand_dist_rand_dist_norm_rand_dist_norm_lognormal_param_ctor_45f379d7 (libcis)

