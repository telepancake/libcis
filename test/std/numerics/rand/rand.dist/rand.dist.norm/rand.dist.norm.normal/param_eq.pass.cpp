// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_norm_rand_dist_norm_normal_param_eq_29072e09).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class RealType = double>
// class normal_distribution
// {
//     class param_type;

#include <random>
#include <limits>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_dist_rand_dist_norm_rand_dist_norm_normal_param_eq_29072e09 { // libcis
int main(int, char**)
{
    {
        typedef std::normal_distribution<> D;
        typedef D::param_type param_type;
        param_type p1(0.75, .5);
        param_type p2(0.75, .5);
        assert(p1 == p2);
    }
    {
        typedef std::normal_distribution<> D;
        typedef D::param_type param_type;
        param_type p1(0.75, .5);
        param_type p2(0.5, .5);
        assert(p1 != p2);
    }

  return 0;

    return 0;
}
} // libcis_ns_numerics_rand_rand_dist_rand_dist_norm_rand_dist_norm_normal_param_eq_29072e09 (libcis)

