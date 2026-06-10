// AST-transferred from libc++ by tools/transfer.py (slug=numerics_complex_number_complex_value_ops_real).
// main -> test_numerics_complex_number_complex_value_ops_real; file-scope helpers isolated in anon namespace.
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
//   real(const complex<T>& x);

#include <complex>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_complex_number_complex_value_ops_real { // libcis: isolate file-scope helpers
template <class T>
void
test()
{
    std::complex<T> z(1.5, 2.5);
    assert(real(z) == 1.5);
}
} using namespace libcis_ns_numerics_complex_number_complex_value_ops_real; // libcis


void test_numerics_complex_number_complex_value_ops_real()
{
    test<float>();
    test<double>();
    test<long double>();

  return;
}
