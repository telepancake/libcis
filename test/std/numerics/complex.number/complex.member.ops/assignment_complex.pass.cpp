// AST-transferred from libc++ by tools/transfer.py (slug=numerics_complex_number_complex_member_ops_assignment_complex).
// main -> test_numerics_complex_number_complex_member_ops_assignment_complex; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <complex>

// complex& operator=(const complex&);
// template<class X> complex& operator= (const complex<X>&); // constexpr in C++20

#include <complex>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_complex_number_complex_member_ops_assignment_complex { // libcis: isolate file-scope helpers
template <class T, class X>
TEST_CONSTEXPR_CXX20
bool
test()
{
    std::complex<T> c;
    assert(c.real() == 0);
    assert(c.imag() == 0);
    std::complex<T> c2(1.5, 2.5);
    c = c2;
    assert(c.real() == 1.5);
    assert(c.imag() == 2.5);
    std::complex<X> c3(3.5, -4.5);
    c = c3;
    assert(c.real() == 3.5);
    assert(c.imag() == -4.5);
    return true;
}
} using namespace libcis_ns_numerics_complex_number_complex_member_ops_assignment_complex; // libcis


void test_numerics_complex_number_complex_member_ops_assignment_complex()
{
    test<float, float>();
    test<float, double>();
    test<float, long double>();

    test<double, float>();
    test<double, double>();
    test<double, long double>();

    test<long double, float>();
    test<long double, double>();
    test<long double, long double>();

#if TEST_STD_VER >= 20
    static_assert(test<float, float>());
    static_assert(test<float, double>());
    static_assert(test<float, long double>());

    static_assert(test<double, float>());
    static_assert(test<double, double>());
    static_assert(test<double, long double>());

    static_assert(test<long double, float>());
    static_assert(test<long double, double>());
    static_assert(test<long double, long double>());
#endif

  return;
}
