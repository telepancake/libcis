// AST-transferred from libc++ by tools/transfer.py (slug=numerics_complex_number_complex_ops_complex_divide_scalar).
// main -> test_numerics_complex_number_complex_ops_complex_divide_scalar; file-scope helpers isolated in anon namespace.
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
//   operator/(const complex<T>& lhs, const T& rhs); // constexpr in C++20

#include <complex>
#include <cassert>

#include "test_macros.h"

namespace { // libcis: isolate file-scope helpers
template <class T>
TEST_CONSTEXPR_CXX20
bool
test()
{
    const std::complex<T> lhs(-4.0, 7.5);
    const T rhs(2);
    assert(lhs / rhs == std::complex<T>(-2, 3.75));
    return true;
}
} // anonymous namespace (libcis)


void test_numerics_complex_number_complex_ops_complex_divide_scalar()
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
