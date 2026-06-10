// AST-transferred from libc++ by tools/transfer.py (slug=numerics_complex_number_complex_transcendentals_exp).
// main -> test_numerics_complex_number_complex_transcendentals_exp; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// Missing some long double math functions.
// XFAIL: LLVM-LIBC-FIXME

// <complex>

// template<class T>
//   complex<T>
//   exp(const complex<T>& x);

#include <complex>
#include <cassert>

#include "test_macros.h"
#include "../cases.h"

namespace libcis_ns_numerics_complex_number_complex_transcendentals_exp { // libcis: isolate file-scope helpers
template <class T>
void
test(const std::complex<T>& c, std::complex<T> x)
{
    assert(exp(c) == x);
}

template <class T>
void
test()
{
    test(std::complex<T>(0, 0), std::complex<T>(1, 0));
}

void test_edges()
{
    const unsigned N = sizeof(testcases) / sizeof(testcases[0]);
    for (unsigned i = 0; i < N; ++i)
    {
        std::complex<double> r = exp(testcases[i]);
        if (testcases[i].real() == 0 && testcases[i].imag() == 0)
        {
            assert(r.real() == 1.0);
            assert(r.imag() == 0);
            assert(std::signbit(testcases[i].imag()) == std::signbit(r.imag()));
        }
        else if (std::isfinite(testcases[i].real()) && std::isinf(testcases[i].imag()))
        {
            assert(std::isnan(r.real()));
            assert(std::isnan(r.imag()));
        }
        else if (std::isfinite(testcases[i].real()) && std::isnan(testcases[i].imag()))
        {
            assert(std::isnan(r.real()));
            assert(std::isnan(r.imag()));
        }
        else if (std::isinf(testcases[i].real()) && testcases[i].real() > 0 && testcases[i].imag() == 0)
        {
            assert(std::isinf(r.real()));
            assert(r.real() > 0);
            assert(r.imag() == 0);
            assert(std::signbit(testcases[i].imag()) == std::signbit(r.imag()));
        }
        else if (std::isinf(testcases[i].real()) && testcases[i].real() < 0 && std::isinf(testcases[i].imag()))
        {
            assert(r.real() == 0);
            assert(r.imag() == 0);
        }
        else if (std::isinf(testcases[i].real()) && testcases[i].real() > 0 && std::isinf(testcases[i].imag()))
        {
            assert(std::isinf(r.real()));
            assert(std::isnan(r.imag()));
        }
        else if (std::isinf(testcases[i].real()) && testcases[i].real() < 0 && std::isnan(testcases[i].imag()))
        {
            assert(r.real() == 0);
            assert(r.imag() == 0);
        }
        else if (std::isinf(testcases[i].real()) && testcases[i].real() > 0 && std::isnan(testcases[i].imag()))
        {
            assert(std::isinf(r.real()));
            assert(std::isnan(r.imag()));
        }
        else if (std::isnan(testcases[i].real()) && testcases[i].imag() == 0)
        {
            assert(std::isnan(r.real()));
            assert(r.imag() == 0);
            assert(std::signbit(testcases[i].imag()) == std::signbit(r.imag()));
        }
        else if (std::isnan(testcases[i].real()) && testcases[i].imag() != 0)
        {
            assert(std::isnan(r.real()));
            assert(std::isnan(r.imag()));
        }
        else if (std::isnan(testcases[i].real()) && std::isnan(testcases[i].imag()))
        {
            assert(std::isnan(r.real()));
            assert(std::isnan(r.imag()));
        }
        else if (std::isfinite(testcases[i].imag()) && std::abs(testcases[i].imag()) <= 1)
        {
            assert(!std::signbit(r.real()));
            assert(std::signbit(r.imag()) == std::signbit(testcases[i].imag()));
        }
        else if (std::isinf(r.real()) && testcases[i].imag() == 0) {
            assert(r.imag() == 0);
            assert(std::signbit(testcases[i].imag()) == std::signbit(r.imag()));
        }
    }
}
} using namespace libcis_ns_numerics_complex_number_complex_transcendentals_exp; // libcis


void test_numerics_complex_number_complex_transcendentals_exp()
{
    test<float>();
    test<double>();
    test<long double>();
    test_edges();

    return;
}
