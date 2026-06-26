// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_poisson_param_ctor_9b6e6815).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class IntType = int>
// class poisson_distribution
// {
//     class param_type;

#include <random>
#include <limits>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_poisson_param_ctor_9b6e6815 { // libcis
int main(int, char**)
{
    {
        typedef std::poisson_distribution<> D;
        typedef D::param_type param_type;
        param_type p;
        assert(p.mean() == 1);
    }
    {
        typedef std::poisson_distribution<> D;
        typedef D::param_type param_type;
        param_type p(10);
        assert(p.mean() == 10);
    }

  return 0;

    return 0;
}
} // libcis_ns_numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_poisson_param_ctor_9b6e6815 (libcis)

