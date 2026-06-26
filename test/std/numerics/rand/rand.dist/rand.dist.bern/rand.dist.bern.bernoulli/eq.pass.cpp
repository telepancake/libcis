// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_bernoulli_eq_8dbd5d7f).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// class bernoulli_distribution

// bool operator=(const bernoulli_distribution& x,
//                const bernoulli_distribution& y);
// bool operator!(const bernoulli_distribution& x,
//                const bernoulli_distribution& y);

#include <random>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_bernoulli_eq_8dbd5d7f { // libcis
int main(int, char**)
{
    {
        typedef std::bernoulli_distribution D;
        D d1(.25);
        D d2(.25);
        assert(d1 == d2);
    }
    {
        typedef std::bernoulli_distribution D;
        D d1(.28);
        D d2(.25);
        assert(d1 != d2);
    }

  return 0;

    return 0;
}
} // libcis_ns_numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_bernoulli_eq_8dbd5d7f (libcis)

