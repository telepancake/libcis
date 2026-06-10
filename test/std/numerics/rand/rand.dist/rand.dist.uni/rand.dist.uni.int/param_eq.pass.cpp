// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_uni_rand_dist_uni_int_param_eq).
// main -> test_numerics_rand_rand_dist_rand_dist_uni_rand_dist_uni_int_param_eq; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class _IntType = int>
// class uniform_int_distribution
// {
//     class param_type;

#include <random>
#include <limits>
#include <cassert>

#include "test_macros.h"

void test_numerics_rand_rand_dist_rand_dist_uni_rand_dist_uni_int_param_eq()
{
    {
        typedef std::uniform_int_distribution<long> D;
        typedef D::param_type param_type;
        param_type p1(5, 10);
        param_type p2(5, 10);
        assert(p1 == p2);
    }
    {
        typedef std::uniform_int_distribution<long> D;
        typedef D::param_type param_type;
        param_type p1(5, 10);
        param_type p2(6, 10);
        assert(p1 != p2);
    }

  return;
}
