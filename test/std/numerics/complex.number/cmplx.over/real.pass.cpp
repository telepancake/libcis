// AST-transferred from libc++ by tools/transfer.py (slug=numerics_complex_number_cmplx_over_real).
// main -> test_numerics_complex_number_cmplx_over_real; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <complex>

// template<Arithmetic T>
//   T
//   real(const T& x);

#include <complex>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "../cases.h"

namespace libcis_ns_numerics_complex_number_cmplx_over_real { // libcis: isolate file-scope helpers
template <class T, int x>
void
test(typename std::enable_if<std::is_integral<T>::value>::type* = 0)
{
    static_assert((std::is_same<decltype(std::real(T(x))), double>::value), "");
    assert(std::real(x) == x);
#if TEST_STD_VER > 11
    constexpr T val {x};
    static_assert(std::real(val) == val, "");
    constexpr std::complex<T> t{val, val};
    static_assert(t.real() == x, "" );
#endif
}

template <class T, int x>
void
test(typename std::enable_if<!std::is_integral<T>::value>::type* = 0)
{
    static_assert((std::is_same<decltype(std::real(T(x))), T>::value), "");
    assert(std::real(x) == x);
#if TEST_STD_VER > 11
    constexpr T val {x};
    static_assert(std::real(val) == val, "");
    constexpr std::complex<T> t{val, val};
    static_assert(t.real() == x, "" );
#endif
}

template <class T>
void
test()
{
    test<T, 0>();
    test<T, 1>();
    test<T, 10>();
}
} using namespace libcis_ns_numerics_complex_number_cmplx_over_real; // libcis


void test_numerics_complex_number_cmplx_over_real()
{
    test<float>();
    test<double>();
    test<long double>();
    test<int>();
    test<unsigned>();
    test<long long>();

  return;
}
