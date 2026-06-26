// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_norm_rand_dist_norm_f_param_ctor_c209fd0d).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class RealType = double>
// class fisher_f_distribution
// {
//     class param_type;

#include <random>
#include <limits>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_dist_rand_dist_norm_rand_dist_norm_f_param_ctor_c209fd0d { // libcis
int main(int, char**)
{
    {
        typedef std::fisher_f_distribution<> D;
        typedef D::param_type param_type;
        param_type p;
        assert(p.m() == 1);
        assert(p.n() == 1);
    }
    {
        typedef std::fisher_f_distribution<> D;
        typedef D::param_type param_type;
        param_type p(10);
        assert(p.m() == 10);
        assert(p.n() == 1);
    }
    {
        typedef std::fisher_f_distribution<> D;
        typedef D::param_type param_type;
        param_type p(10, 5);
        assert(p.m() == 10);
        assert(p.n() == 5);
    }

  return 0;

    return 0;
}
} // libcis_ns_numerics_rand_rand_dist_rand_dist_norm_rand_dist_norm_f_param_ctor_c209fd0d (libcis)

