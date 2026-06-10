// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_negbin_max).
// main -> test_numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_negbin_max; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class IntType = int>
// class negative_binomial_distribution

// result_type max() const;

#include <random>

#include <cassert>
#include <limits>

#include "test_macros.h"

void test_numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_negbin_max()
{
    {
        typedef std::negative_binomial_distribution<> D;
        D d(4, .25);
        assert(d.max() == std::numeric_limits<int>::max());
    }

  return;
}
