// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_norm_rand_dist_norm_lognormal_param_assign_f4b8a21e).
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

namespace libcis_ns_numerics_rand_rand_dist_rand_dist_norm_rand_dist_norm_lognormal_param_assign_f4b8a21e { // libcis
int main(int, char**)
{
    {
        typedef std::lognormal_distribution<> D;
        typedef D::param_type param_type;
        param_type p0(.75, 6);
        param_type p;
        p = p0;
        assert(p.m() == .75);
        assert(p.s() == 6);
    }

  return 0;

    return 0;
}
} // libcis_ns_numerics_rand_rand_dist_rand_dist_norm_rand_dist_norm_lognormal_param_assign_f4b8a21e (libcis)

