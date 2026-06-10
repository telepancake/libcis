// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_samp_rand_dist_samp_discrete_ctor_init).
// main -> test_numerics_rand_rand_dist_rand_dist_samp_rand_dist_samp_discrete_ctor_init; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <random>

// template<class IntType = int>
// class discrete_distribution

// discrete_distribution(initializer_list<double> wl);

#include <random>

#include <cassert>
#include <vector>

#include "test_macros.h"

void test_numerics_rand_rand_dist_rand_dist_samp_rand_dist_samp_discrete_ctor_init()
{
    {
        typedef std::discrete_distribution<> D;
        D d = {};
        std::vector<double> p = d.probabilities();
        assert(p.size() == 1);
        assert(p[0] == 1);
    }
    {
        typedef std::discrete_distribution<> D;
        D d = {10};
        std::vector<double> p = d.probabilities();
        assert(p.size() == 1);
        assert(p[0] == 1);
    }
    {
        typedef std::discrete_distribution<> D;
        D d = {10, 30};
        std::vector<double> p = d.probabilities();
        assert(p.size() == 2);
        assert(p[0] == 0.25);
        assert(p[1] == 0.75);
    }
    {
        typedef std::discrete_distribution<> D;
        D d = {30, 10};
        std::vector<double> p = d.probabilities();
        assert(p.size() == 2);
        assert(p[0] == 0.75);
        assert(p[1] == 0.25);
    }
    {
        typedef std::discrete_distribution<> D;
        D d = {30, 0, 10};
        std::vector<double> p = d.probabilities();
        assert(p.size() == 3);
        assert(p[0] == 0.75);
        assert(p[1] == 0);
        assert(p[2] == 0.25);
    }
    {
        typedef std::discrete_distribution<> D;
        D d = {0, 30, 10};
        std::vector<double> p = d.probabilities();
        assert(p.size() == 3);
        assert(p[0] == 0);
        assert(p[1] == 0.75);
        assert(p[2] == 0.25);
    }
    {
        typedef std::discrete_distribution<> D;
        D d = {0, 0, 10};
        std::vector<double> p = d.probabilities();
        assert(p.size() == 3);
        assert(p[0] == 0);
        assert(p[1] == 0);
        assert(p[2] == 1);
    }

  return;
}
