// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_weibull_param_ctor_b9b70a2b).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class RealType = double>
// class weibull_distribution
// {
//     class param_type;

#include <random>
#include <limits>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_weibull_param_ctor_b9b70a2b { // libcis
int main(int, char**)
{
    {
        typedef std::weibull_distribution<> D;
        typedef D::param_type param_type;
        param_type p;
        assert(p.a() == 1);
        assert(p.b() == 1);
    }
    {
        typedef std::weibull_distribution<> D;
        typedef D::param_type param_type;
        param_type p(10);
        assert(p.a() == 10);
        assert(p.b() == 1);
    }
    {
        typedef std::weibull_distribution<> D;
        typedef D::param_type param_type;
        param_type p(10, 5);
        assert(p.a() == 10);
        assert(p.b() == 5);
    }

  return 0;

    return 0;
}
} // libcis_ns_numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_weibull_param_ctor_b9b70a2b (libcis)

