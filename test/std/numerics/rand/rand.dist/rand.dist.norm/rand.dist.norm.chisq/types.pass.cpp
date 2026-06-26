// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_norm_rand_dist_norm_chisq_types_9adabe99).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class RealType = double>
// class chi_squared_distribution
// {
// public:
//     // types
//     typedef RealType result_type;

#include <random>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_dist_rand_dist_norm_rand_dist_norm_chisq_types_9adabe99 { // libcis
int main(int, char**)
{
    {
        typedef std::chi_squared_distribution<> D;
        typedef D::result_type result_type;
        static_assert((std::is_same<result_type, double>::value), "");
    }
    {
        typedef std::chi_squared_distribution<float> D;
        typedef D::result_type result_type;
        static_assert((std::is_same<result_type, float>::value), "");
    }

  return 0;

    return 0;
}
} // libcis_ns_numerics_rand_rand_dist_rand_dist_norm_rand_dist_norm_chisq_types_9adabe99 (libcis)

