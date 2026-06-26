// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_uni_rand_dist_uni_real_param_copy_5163c3d2).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class RealType = double>
// class uniform_real_distribution
// {
//     class param_type;

#include <random>
#include <limits>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_dist_rand_dist_uni_rand_dist_uni_real_param_copy_5163c3d2 { // libcis
int main(int, char**)
{
    {
        typedef std::uniform_real_distribution<float> D;
        typedef D::param_type param_type;
        param_type p0(5, 10);
        param_type p = p0;
        assert(p.a() == 5);
        assert(p.b() == 10);
    }

  return 0;

    return 0;
}
} // libcis_ns_numerics_rand_rand_dist_rand_dist_uni_rand_dist_uni_real_param_copy_5163c3d2 (libcis)

