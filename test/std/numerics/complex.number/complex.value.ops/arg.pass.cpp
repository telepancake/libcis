// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_complex_number_complex_value_ops_arg_422a1861).
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
//   arg(const complex<T>& x);

#include <complex>
#include <cassert>

#include "test_macros.h"
#include "../cases.h"

namespace libcis_ns_numerics_complex_number_complex_value_ops_arg_422a1861 { // libcis
template <class T>
void
test()
{
    std::complex<T> z(1, 0);
    assert(arg(z) == 0);
}

void test_edges()
{
    const double pi = std::atan2(+0., -0.);
    const unsigned N = sizeof(testcases) / sizeof(testcases[0]);
    for (unsigned i = 0; i < N; ++i)
    {
        double r = arg(testcases[i]);
        if (std::isnan(testcases[i].real()) || std::isnan(testcases[i].imag()))
            assert(std::isnan(r));
        else
        {
            switch (classify(testcases[i]))
            {
            case zero:
                if (std::signbit(testcases[i].real()))
                {
                    if (std::signbit(testcases[i].imag()))
                        is_about(r, -pi);
                    else
                        is_about(r, pi);
                }
                else
                {
                    assert(std::signbit(testcases[i].imag()) == std::signbit(r));
                }
                break;
            case non_zero:
                if (testcases[i].real() == 0)
                {
                    if (testcases[i].imag() < 0)
                        is_about(r, -pi/2);
                    else
                        is_about(r, pi/2);
                }
                else if (testcases[i].imag() == 0)
                {
                    if (testcases[i].real() < 0)
                    {
                        if (std::signbit(testcases[i].imag()))
                            is_about(r, -pi);
                        else
                            is_about(r, pi);
                    }
                    else
                    {
                        assert(r == 0);
                        assert(std::signbit(testcases[i].imag()) == std::signbit(r));
                    }
                }
                else if (testcases[i].imag() > 0)
                    assert(r > 0);
                else
                    assert(r < 0);
                break;
            case inf:
                if (std::isinf(testcases[i].real()) && std::isinf(testcases[i].imag()))
                {
                    if (testcases[i].real() < 0)
                    {
                        if (testcases[i].imag() > 0)
                            is_about(r, 0.75 * pi);
                        else
                            is_about(r, -0.75 * pi);
                    }
                    else
                    {
                        if (testcases[i].imag() > 0)
                            is_about(r, 0.25 * pi);
                        else
                            is_about(r, -0.25 * pi);
                    }
                }
                else if (std::isinf(testcases[i].real()))
                {
                    if (testcases[i].real() < 0)
                    {
                        if (std::signbit(testcases[i].imag()))
                            is_about(r, -pi);
                        else
                            is_about(r, pi);
                    }
                    else
                    {
                        assert(r == 0);
                        assert(std::signbit(r) == std::signbit(testcases[i].imag()));
                    }
                }
                else
                {
                    if (testcases[i].imag() < 0)
                        is_about(r, -pi/2);
                    else
                        is_about(r, pi/2);
                }
                break;
            }
        }
    }
}

int main(int, char**)
{
    test<float>();
    test<double>();
    test<long double>();
    test_edges();

  return 0;

    return 0;
}
} // libcis_ns_numerics_complex_number_complex_value_ops_arg_422a1861 (libcis)

