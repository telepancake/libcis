// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_uni_rand_dist_uni_real_types).
// main -> test_numerics_rand_rand_dist_rand_dist_uni_rand_dist_uni_real_types; file-scope helpers isolated in anon namespace.
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
// {
//     typedef IntType result_type;

#include <random>
#include <type_traits>

#include "test_macros.h"

void test_numerics_rand_rand_dist_rand_dist_uni_rand_dist_uni_real_types()
{
    {
        typedef std::uniform_real_distribution<float> D;
        typedef D::result_type result_type;
        static_assert((std::is_same<result_type, float>::value), "");
    }

  return;
}
