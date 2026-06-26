// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_uni_rand_dist_uni_real_max_95730b83).
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

// result_type max() const;

#include <random>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_dist_rand_dist_uni_rand_dist_uni_real_max_95730b83 { // libcis
int main(int, char**)
{
    {
        typedef std::uniform_real_distribution<> D;
        D d(3, 8);
        assert(d.max() == 8);
    }

  return 0;

    return 0;
}
} // libcis_ns_numerics_rand_rand_dist_rand_dist_uni_rand_dist_uni_real_max_95730b83 (libcis)

