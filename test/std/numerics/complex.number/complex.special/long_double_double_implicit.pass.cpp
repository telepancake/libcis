// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_complex_number_complex_special_long_double_double_implicit_37f3347a).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <complex>

// template<> class complex<long double>
// {
// public:
//     constexpr complex(const complex<double>&);
// };

#include <complex>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_complex_number_complex_special_long_double_double_implicit_37f3347a { // libcis
int main(int, char**)
{
    {
    const std::complex<double> cd(2.5, 3.5);
    std::complex<long double> cf = cd;
    assert(cf.real() == cd.real());
    assert(cf.imag() == cd.imag());
    }
#if TEST_STD_VER >= 11
    {
    constexpr std::complex<double> cd(2.5, 3.5);
    constexpr std::complex<long double> cf = cd;
    static_assert(cf.real() == cd.real(), "");
    static_assert(cf.imag() == cd.imag(), "");
    }
#endif

  return 0;

    return 0;
}
} // libcis_ns_numerics_complex_number_complex_special_long_double_double_implicit_37f3347a (libcis)

