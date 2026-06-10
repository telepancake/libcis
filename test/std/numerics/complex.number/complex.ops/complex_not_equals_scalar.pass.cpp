// AST-transferred from libc++ by tools/transfer.py (slug=numerics_complex_number_complex_ops_complex_not_equals_scalar).
// main -> test_numerics_complex_number_complex_ops_complex_not_equals_scalar; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <complex>

// template<class T>
//   bool
//   operator!=(const complex<T>& lhs, const T& rhs);

#include <complex>
#include <cassert>

#include "test_macros.h"

namespace { // libcis: isolate file-scope helpers
template <class T>
TEST_CONSTEXPR_CXX20
void
test_constexpr()
{
#if TEST_STD_VER > 11
    {
    constexpr std::complex<T> lhs(1.5,  2.5);
    constexpr T rhs(-2.5);
    static_assert(lhs != rhs, "");
    }
    {
    constexpr std::complex<T> lhs(1.5,  0);
    constexpr T rhs(-2.5);
    static_assert(lhs != rhs, "");
    }
    {
    constexpr std::complex<T> lhs(1.5, 2.5);
    constexpr T rhs(1.5);
    static_assert(lhs != rhs, "");
    }
    {
    constexpr std::complex<T> lhs(1.5, 0);
    constexpr T rhs(1.5);
    static_assert( !(lhs != rhs), "");
    }
#endif
}

template <class T>
TEST_CONSTEXPR_CXX20
bool
test()
{
    {
    std::complex<T> lhs(1.5,  2.5);
    T rhs(-2.5);
    assert(lhs != rhs);
    }
    {
    std::complex<T> lhs(1.5,  0);
    T rhs(-2.5);
    assert(lhs != rhs);
    }
    {
    std::complex<T> lhs(1.5, 2.5);
    T rhs(1.5);
    assert(lhs != rhs);
    }
    {
    std::complex<T> lhs(1.5, 0);
    T rhs(1.5);
    assert( !(lhs != rhs));
    }

    test_constexpr<T> ();
    return true;
}
} // anonymous namespace (libcis)


void test_numerics_complex_number_complex_ops_complex_not_equals_scalar()
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
