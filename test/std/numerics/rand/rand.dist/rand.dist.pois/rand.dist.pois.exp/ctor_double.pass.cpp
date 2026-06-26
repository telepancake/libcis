// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_exp_ctor_double_5ac2b194).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class RealType = double>
// class exponential_distribution

// explicit exponential_distribution(RealType lambda = 1.0);     // before C++20
// exponential_distribution() : exponential_distribution(1.0) {} // C++20
// explicit exponential_distribution(RealType lambda);           // C++20

#include <random>
#include <cassert>

#include "test_macros.h"
#if TEST_STD_VER >= 11
#include "make_implicit.h"
#include "test_convertible.h"
#endif

namespace libcis_ns_numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_exp_ctor_double_5ac2b194 { // libcis
template <class T>
void test_implicit() {
#if TEST_STD_VER >= 11
  typedef std::exponential_distribution<T> D;
  static_assert(test_convertible<D>(), "");
  assert(D(1) == make_implicit<D>());
  static_assert(!test_convertible<D, T>(), "");
#endif
}

int main(int, char**)
{
    {
        typedef std::exponential_distribution<> D;
        D d;
        assert(d.lambda() == 1);
    }
    {
        typedef std::exponential_distribution<> D;
        D d(3.5);
        assert(d.lambda() == 3.5);
    }

    test_implicit<float>();
    test_implicit<double>();

    return 0;

    return 0;
}
} // libcis_ns_numerics_rand_rand_dist_rand_dist_pois_rand_dist_pois_exp_ctor_double_5ac2b194 (libcis)

