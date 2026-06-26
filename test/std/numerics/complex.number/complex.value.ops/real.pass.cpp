// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_complex_number_complex_value_ops_real_2c307df6).
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

namespace libcis_ns_numerics_complex_number_complex_value_ops_real_2c307df6 { // libcis
template <class T>
void
test()
{
    std::complex<T> z(1.5, 2.5);
    assert(real(z) == 1.5);
}

int main(int, char**)
{
    test<float>();
    test<double>();
    test<long double>();

  return 0;

    return 0;
}
} // libcis_ns_numerics_complex_number_complex_value_ops_real_2c307df6 (libcis)

