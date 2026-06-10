// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_geo_ctor_double).
// main -> test_numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_geo_ctor_double; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class IntType = int>
// class geometric_distribution

// explicit geometric_distribution(double p = 0.5);          // before C++20
// geometric_distribution() : geometric_distribution(0.5) {} // C++20
// explicit geometric_distribution(double p);                // C++20

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
  typedef std::geometric_distribution<T> D;
  static_assert(test_convertible<D>(), "");
  assert(D(0.5) == make_implicit<D>());
  static_assert(!test_convertible<D, double>(), "");
#endif
}
} // anonymous namespace (libcis)


void test_numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_geo_ctor_double()
{
    {
        typedef std::geometric_distribution<> D;
        D d;
        assert(d.p() == 0.5);
    }
    {
        typedef std::geometric_distribution<> D;
        D d(0.75);
        assert(d.p() == 0.75);
    }

    test_implicit<int>();
    test_implicit<long>();

    return;
}
