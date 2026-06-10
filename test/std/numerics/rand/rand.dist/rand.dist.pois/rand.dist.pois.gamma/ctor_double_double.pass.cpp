// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_gamma_ctor_double_double).
// main -> test_numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_gamma_ctor_double_double; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class RealType = double>
// class gamma_distribution

// explicit gamma_distribution(RealType alpha = 1.0, RealType beta = 1.0); // before C++20
// gamma_distribution() : gamma_distribution(1.0) {}                       // C++20
// explicit gamma_distribution(RealType alpha, RealType beta = 1.0);       // C++20

#include <random>
#include <cassert>

#include "test_macros.h"
#if TEST_STD_VER >= 11
#include "make_implicit.h"
#include "test_convertible.h"
#endif

namespace libcis_ns_numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_gamma_ctor_double_double { // libcis: isolate file-scope helpers
template <class T>
void test_implicit() {
#if TEST_STD_VER >= 11
  typedef std::gamma_distribution<T> D;
  static_assert(test_convertible<D>(), "");
  assert(D(1) == make_implicit<D>());
  static_assert(!test_convertible<D, T>(), "");
  static_assert(!test_convertible<D, T, T>(), "");
#endif
}
} using namespace libcis_ns_numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_gamma_ctor_double_double; // libcis


void test_numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_gamma_ctor_double_double()
{
    {
        typedef std::gamma_distribution<> D;
        D d;
        assert(d.alpha() == 1);
        assert(d.beta() == 1);
    }
    {
        typedef std::gamma_distribution<> D;
        D d(14.5);
        assert(d.alpha() == 14.5);
        assert(d.beta() == 1);
    }
    {
        typedef std::gamma_distribution<> D;
        D d(14.5, 5.25);
        assert(d.alpha() == 14.5);
        assert(d.beta() == 5.25);
    }

    test_implicit<float>();
    test_implicit<double>();

    return;
}
