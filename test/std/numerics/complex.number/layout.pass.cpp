// AST-transferred from libc++ by tools/transfer.py (slug=numerics_complex_number_layout).
// main -> test_numerics_complex_number_layout; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <complex>

#include <complex>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_complex_number_layout { // libcis: isolate file-scope helpers
template <class T>
void
test()
{
    std::complex<T> z;
    T* a = (T*)&z;
    assert(0 == z.real());
    assert(0 == z.imag());
    assert(a[0] == z.real());
    assert(a[1] == z.imag());
    a[0] = 5;
    a[1] = 6;
    assert(a[0] == z.real());
    assert(a[1] == z.imag());
}
} using namespace libcis_ns_numerics_complex_number_layout; // libcis


void test_numerics_complex_number_layout()
{
    test<float>();
    test<double>();
    test<long double>();

  return;
}
