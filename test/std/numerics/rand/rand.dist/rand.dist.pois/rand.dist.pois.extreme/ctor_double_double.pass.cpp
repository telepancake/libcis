// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_extreme_ctor_double_double).
// main -> test_numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_extreme_ctor_double_double; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class RealType = double>
// class extreme_value_distribution

// explicit extreme_value_distribution(RealType a = 0.0, RealType b = 1.0); // before C++20
// extreme_value_distribution() : extreme_value_distribution(0.0) {}        // C++20
// explicit extreme_value_distribution(RealType a, RealType b = 1.0);       // C++20

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
  typedef std::extreme_value_distribution<T> D;
  static_assert(test_convertible<D>(), "");
  assert(D(0) == make_implicit<D>());
  static_assert(!test_convertible<D, T>(), "");
  static_assert(!test_convertible<D, T, T>(), "");
#endif
}
} // anonymous namespace (libcis)


void test_numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_extreme_ctor_double_double()
{
    {
        typedef std::extreme_value_distribution<> D;
        D d;
        assert(d.a() == 0);
        assert(d.b() == 1);
    }
    {
        typedef std::extreme_value_distribution<> D;
        D d(14.5);
        assert(d.a() == 14.5);
        assert(d.b() == 1);
    }
    {
        typedef std::extreme_value_distribution<> D;
        D d(14.5, 5.25);
        assert(d.a() == 14.5);
        assert(d.b() == 5.25);
    }

    test_implicit<float>();
    test_implicit<double>();

    return;
}
