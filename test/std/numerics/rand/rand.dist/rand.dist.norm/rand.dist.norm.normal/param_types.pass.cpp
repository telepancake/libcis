// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_norm_rand_dist_norm_normal_param_types).
// main -> test_numerics_rand_rand_dist_rand_dist_norm_rand_dist_norm_normal_param_types; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class RealType = double>
// class normal_distribution
// {
//     class param_type;

#include <random>
#include <type_traits>

#include "test_macros.h"

void test_numerics_rand_rand_dist_rand_dist_norm_rand_dist_norm_normal_param_types()
{
    {
        typedef std::normal_distribution<> D;
        typedef D::param_type param_type;
        typedef param_type::distribution_type distribution_type;
        static_assert((std::is_same<D, distribution_type>::value), "");
    }

  return;
}
