// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_samp_rand_dist_samp_pconst_ctor_default).
// main -> test_numerics_rand_rand_dist_rand_dist_samp_rand_dist_samp_pconst_ctor_default; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <random>

// template<class RealType = double>
// class piecewise_constant_distribution

// piecewise_constant_distribution(initializer_list<double> wl);

#include <random>

#include <cassert>
#include <vector>

#include "test_macros.h"

void test_numerics_rand_rand_dist_rand_dist_samp_rand_dist_samp_pconst_ctor_default()
{
    {
        typedef std::piecewise_constant_distribution<> D;
        D d;
        std::vector<double> iv = d.intervals();
        assert(iv.size() == 2);
        assert(iv[0] == 0);
        assert(iv[1] == 1);
        std::vector<double> dn = d.densities();
        assert(dn.size() == 1);
        assert(dn[0] == 1);
    }

  return;
}
