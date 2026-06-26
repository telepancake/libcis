// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_samp_rand_dist_samp_discrete_ctor_param_8a2b2da1).
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

// explicit discrete_distribution(const param_type& parm);

#include <random>

#include <cassert>
#include <vector>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_dist_rand_dist_samp_rand_dist_samp_discrete_ctor_param_8a2b2da1 { // libcis
int main(int, char**)
{
    {
        typedef std::discrete_distribution<> D;
        typedef D::param_type P;
        double p0[] = {10, 30};
        P pa(p0, p0+2);
        D d(pa);
        std::vector<double> p = d.probabilities();
        assert(p.size() == 2);
        assert(p[0] == 0.25);
        assert(p[1] == 0.75);
    }

  return 0;

    return 0;
}
} // libcis_ns_numerics_rand_rand_dist_rand_dist_samp_rand_dist_samp_discrete_ctor_param_8a2b2da1 (libcis)

