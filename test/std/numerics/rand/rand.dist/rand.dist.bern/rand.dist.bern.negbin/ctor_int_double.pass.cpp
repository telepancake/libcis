// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_negbin_ctor_int_double).
// main -> test_numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_negbin_ctor_int_double; file-scope helpers isolated in anon namespace.
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

// explicit negative_binomial_distribution(IntType k = 1, double p = 0.5); // before C++20
// negative_binomial_distribution() : negative_binomial_distribution(1) {} // C++20
// explicit negative_binomial_distribution(IntType k, double p = 0.5);     // C++20

#include <random>
#include <cassert>

#include "test_macros.h"
#if TEST_STD_VER >= 11
#include "make_implicit.h"
#include "test_convertible.h"
#endif

namespace { // libcis: isolate file-scope helpers
template <class T>
void test_implicit() {
#if TEST_STD_VER >= 11
  typedef std::negative_binomial_distribution<T> D;
  static_assert(test_convertible<D>(), "");
  assert(D(1) == make_implicit<D>());
  static_assert(!test_convertible<D, T>(), "");
  static_assert(!test_convertible<D, T, double>(), "");
#endif
}
} // anonymous namespace (libcis)


void test_numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_negbin_ctor_int_double()
{
    {
        typedef std::negative_binomial_distribution<> D;
        D d;
        assert(d.k() == 1);
        assert(d.p() == 0.5);
    }
    {
        typedef std::negative_binomial_distribution<> D;
        D d(3);
        assert(d.k() == 3);
        assert(d.p() == 0.5);
    }
    {
        typedef std::negative_binomial_distribution<> D;
        D d(3, 0.75);
        assert(d.k() == 3);
        assert(d.p() == 0.75);
    }

    test_implicit<int>();
    test_implicit<long>();

    return;
}
