// AST-transferred from libc++ by tools/transfer.py (slug=numerics_complex_number_complex_value_ops_conj).
// main -> test_numerics_complex_number_complex_value_ops_conj; file-scope helpers isolated in anon namespace.
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
//   conj(const complex<T>& x);

#include <complex>
#include <cassert>

#include "test_macros.h"

namespace { // libcis: isolate file-scope helpers
template <class T>
void
test(const std::complex<T>& z, std::complex<T> x)
{
    assert(conj(z) == x);
}

template <class T>
void
test()
{
    test(std::complex<T>(1, 2), std::complex<T>(1, -2));
    test(std::complex<T>(-1, 2), std::complex<T>(-1, -2));
    test(std::complex<T>(1, -2), std::complex<T>(1, 2));
    test(std::complex<T>(-1, -2), std::complex<T>(-1, 2));
}
} // anonymous namespace (libcis)


void test_numerics_complex_number_complex_value_ops_conj()
{
    test<float>();
    test<double>();
    test<long double>();

  return;
}
