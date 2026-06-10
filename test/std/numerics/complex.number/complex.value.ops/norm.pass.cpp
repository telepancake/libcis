// AST-transferred from libc++ by tools/transfer.py (slug=numerics_complex_number_complex_value_ops_norm).
// main -> test_numerics_complex_number_complex_value_ops_norm; file-scope helpers isolated in anon namespace.
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
//   norm(const complex<T>& x);

#include <complex>
#include <cassert>

#include "test_macros.h"
#include "../cases.h"

namespace { // libcis: isolate file-scope helpers
template <class T>
void
test()
{
    std::complex<T> z(3, 4);
    assert(norm(z) == 25);
}

void test_edges()
{
    const unsigned N = sizeof(testcases) / sizeof(testcases[0]);
    for (unsigned i = 0; i < N; ++i)
    {
        double r = norm(testcases[i]);
        switch (classify(testcases[i]))
        {
        case zero:
            assert(r == 0);
            assert(!std::signbit(r));
            break;
        case non_zero:
            assert(std::isfinite(r) && r > 0);
            break;
        case inf:
            assert(std::isinf(r) && r > 0);
            break;
        case NaN:
            assert(std::isnan(r));
            break;
        case non_zero_nan:
            assert(std::isnan(r));
            break;
        }
    }
}
} // anonymous namespace (libcis)


void test_numerics_complex_number_complex_value_ops_norm()
{
    test<float>();
    test<double>();
    test<long double>();
    test_edges();

  return;
}
