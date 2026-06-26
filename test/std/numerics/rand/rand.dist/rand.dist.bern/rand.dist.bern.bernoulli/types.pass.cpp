// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_bernoulli_types_c5718b91).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// class bernoulli_distribution
// {
//     typedef bool result_type;

#include <random>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_bernoulli_types_c5718b91 { // libcis
int main(int, char**)
{
    {
        typedef std::bernoulli_distribution D;
        typedef D::result_type result_type;
        static_assert((std::is_same<result_type, bool>::value), "");
    }

  return 0;

    return 0;
}
} // libcis_ns_numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_bernoulli_types_c5718b91 (libcis)

