// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_samp_rand_dist_samp_discrete_param_copy_179c40a1).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class IntType = int>
// class discrete_distribution
// {
//     class param_type;

#include <random>
#include <limits>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_dist_rand_dist_samp_rand_dist_samp_discrete_param_copy_179c40a1 { // libcis
int main(int, char**)
{
    {
        typedef std::discrete_distribution<> D;
        typedef D::param_type param_type;
        double d0[] = {.3, .1, .6};
        param_type p0(d0, d0+3);
        param_type p = p0;
        assert(p == p0);
    }

  return 0;

    return 0;
}
} // libcis_ns_numerics_rand_rand_dist_rand_dist_samp_rand_dist_samp_discrete_param_copy_179c40a1 (libcis)

