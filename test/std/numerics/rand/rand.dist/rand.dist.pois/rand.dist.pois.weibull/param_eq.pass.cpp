// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_weibull_param_eq_6fc9505a).
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

namespace libcis_ns_numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_weibull_param_eq_6fc9505a { // libcis
int main(int, char**)
{
    {
        typedef std::weibull_distribution<> D;
        typedef D::param_type param_type;
        param_type p1(0.75, .5);
        param_type p2(0.75, .5);
        assert(p1 == p2);
    }
    {
        typedef std::weibull_distribution<> D;
        typedef D::param_type param_type;
        param_type p1(0.75, .5);
        param_type p2(0.5, .5);
        assert(p1 != p2);
    }

  return 0;

    return 0;
}
} // libcis_ns_numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_weibull_param_eq_6fc9505a (libcis)

