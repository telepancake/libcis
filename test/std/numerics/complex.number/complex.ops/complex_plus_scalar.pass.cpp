// AST-transferred from libc++ by tools/transfer.py (slug=numerics_complex_number_complex_ops_complex_plus_scalar).
// main -> test_numerics_complex_number_complex_ops_complex_plus_scalar; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <complex>

// template<class T>
//   complex<T>
//   operator+(const complex<T>& lhs, const T& rhs); // constexpr in C++20

#include <complex>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_complex_number_complex_ops_complex_plus_scalar { // libcis: isolate file-scope helpers
template <class T>
TEST_CONSTEXPR_CXX20
bool
test()
{
    {
    const std::complex<T> lhs(1.5, 2.5);
    const T rhs(3.5);
    assert(lhs + rhs == std::complex<T>(5.0, 2.5));
    }
    {
    const std::complex<T> lhs(1.5, -2.5);
    const T rhs(-3.5);
    assert(lhs + rhs == std::complex<T>(-2.0, -2.5));
    }

    return true;
}
} using namespace libcis_ns_numerics_complex_number_complex_ops_complex_plus_scalar; // libcis


void test_numerics_complex_number_complex_ops_complex_plus_scalar()
{
    test<float>();
    test<double>();
    test<long double>();

#if TEST_STD_VER > 17
    static_assert(test<float>());
    static_assert(test<double>());
    static_assert(test<long double>());
#endif

  return;
}
