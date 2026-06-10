// AST-transferred from libc++ by tools/transfer.py (slug=numerics_complex_number_complex_value_ops_imag).
// main -> test_numerics_complex_number_complex_value_ops_imag; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <complex>

// template<class T>
//   T
//   imag(const complex<T>& x);

#include <complex>
#include <cassert>

#include "test_macros.h"

namespace { // libcis: isolate file-scope helpers
template <class T>
void
test()
{
    std::complex<T> z(1.5, 2.5);
    assert(imag(z) == 2.5);
}
} // anonymous namespace (libcis)


void test_numerics_complex_number_complex_value_ops_imag()
{
    test<float>();
    test<double>();
    test<long double>();

  return;
}
