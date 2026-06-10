// AST-transferred from libc++ by tools/transfer.py (slug=numerics_complex_number_complex_member_ops_plus_equal_scalar).
// main -> test_numerics_complex_number_complex_member_ops_plus_equal_scalar; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <complex>

// complex& operator+=(const T& rhs); // constexpr in C++20

#include <complex>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_complex_number_complex_member_ops_plus_equal_scalar { // libcis: isolate file-scope helpers
template <class T>
TEST_CONSTEXPR_CXX20
bool
test()
{
    std::complex<T> c;
    assert(c.real() == 0);
    assert(c.imag() == 0);
    c += 1.5;
    assert(c.real() == 1.5);
    assert(c.imag() == 0);
    c += 1.5;
    assert(c.real() == 3);
    assert(c.imag() == 0);
    c += -1.5;
    assert(c.real() == 1.5);
    assert(c.imag() == 0);
    return true;
}
} using namespace libcis_ns_numerics_complex_number_complex_member_ops_plus_equal_scalar; // libcis


void test_numerics_complex_number_complex_member_ops_plus_equal_scalar()
{
    test<float>();
    test<double>();
    test<long double>();

#if TEST_STD_VER >= 20
    static_assert(test<float>());
    static_assert(test<double>());
    static_assert(test<long double>());
#endif

  return;
}
