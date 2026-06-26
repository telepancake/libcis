// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_bernoulli_set_param_37f82621).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// class bernoulli_distribution

// void param(const param_type& parm);

#include <random>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_bernoulli_set_param_37f82621 { // libcis
int main(int, char**)
{
    {
        typedef std::bernoulli_distribution D;
        typedef D::param_type P;
        P p(0.25);
        D d(0.75);
        d.param(p);
        assert(d.param() == p);
    }

  return 0;

    return 0;
}
} // libcis_ns_numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_bernoulli_set_param_37f82621 (libcis)

