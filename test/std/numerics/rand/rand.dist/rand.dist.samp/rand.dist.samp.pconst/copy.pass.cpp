// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_samp_rand_dist_samp_pconst_copy).
// main -> test_numerics_rand_rand_dist_rand_dist_samp_rand_dist_samp_pconst_copy; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class RealType = double>
// class piecewise_constant_distribution

// piecewise_constant_distribution(const piecewise_constant_distribution&);

#include <random>
#include <cassert>

#include "test_macros.h"

namespace { // libcis: isolate file-scope helpers
void
test1()
{
    typedef std::piecewise_constant_distribution<> D;
    double p[] = {2, 4, 1, 8};
    double b[] = {2, 4, 5, 8, 9};
    D d1(b, b+5, p);
    D d2 = d1;
    assert(d1 == d2);
}
} // anonymous namespace (libcis)


void test_numerics_rand_rand_dist_rand_dist_samp_rand_dist_samp_pconst_copy()
{
    test1();

  return;
}
