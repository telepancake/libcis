// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_complex_number_complex_value_ops_conj_5c278a8d).
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

namespace libcis_ns_numerics_complex_number_complex_value_ops_conj_5c278a8d { // libcis
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

int main(int, char**)
{
    test<float>();
    test<double>();
    test<long double>();

  return 0;

    return 0;
}
} // libcis_ns_numerics_complex_number_complex_value_ops_conj_5c278a8d (libcis)

