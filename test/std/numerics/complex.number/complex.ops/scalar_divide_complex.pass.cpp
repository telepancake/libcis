// AST-transferred from libc++ by tools/transfer.py (slug=numerics_complex_number_complex_ops_scalar_divide_complex).
// main -> test_numerics_complex_number_complex_ops_scalar_divide_complex; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// Missing some math functions that compiler-rt buildins need.
// XFAIL: LLVM-LIBC-FIXME

// <complex>

// template<class T>
//   complex<T>
//   operator/(const T& lhs, const complex<T>& rhs); // constexpr in C++20

#include <complex>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_complex_number_complex_ops_scalar_divide_complex { // libcis: isolate file-scope helpers
template <class T>
TEST_CONSTEXPR_CXX20
bool
test()
{
    const T lhs(-8.5);
    const std::complex<T> rhs(1.5, 2.5);
    const std::complex<T> c = lhs / rhs;
    assert(c.real() >= T(-1.500000000000001));
    assert(c.real() <= T(-1.499999999999999));
    assert(c.imag() >= T(2.499999999999999));
    assert(c.imag() <= T(2.500000000000001));
    return true;
}
} using namespace libcis_ns_numerics_complex_number_complex_ops_scalar_divide_complex; // libcis


void test_numerics_complex_number_complex_ops_scalar_divide_complex()
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
