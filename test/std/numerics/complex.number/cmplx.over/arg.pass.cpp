// AST-transferred from libc++ by tools/transfer.py (slug=numerics_complex_number_cmplx_over_arg).
// main -> test_numerics_complex_number_cmplx_over_arg; file-scope helpers isolated in anon namespace.
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
//   arg(T x);

#include <complex>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "../cases.h"

namespace { // libcis: isolate file-scope helpers
template <class T>
void
test(T x, typename std::enable_if<std::is_integral<T>::value>::type* = 0)
{
    static_assert((std::is_same<decltype(std::arg(x)), double>::value), "");
    assert(std::arg(x) == arg(std::complex<double>(static_cast<double>(x), 0)));
}

template <class T>
void
test(T x, typename std::enable_if<!std::is_integral<T>::value>::type* = 0)
{
    static_assert((std::is_same<decltype(std::arg(x)), T>::value), "");
    assert(std::arg(x) == arg(std::complex<T>(x, 0)));
}

template <class T>
void
test()
{
    test<T>(0);
    test<T>(1);
    test<T>(10);
}
} // anonymous namespace (libcis)


void test_numerics_complex_number_cmplx_over_arg()
{
    test<float>();
    test<double>();
    test<long double>();
    test<int>();
    test<unsigned>();
    test<long long>();

  return;
}
