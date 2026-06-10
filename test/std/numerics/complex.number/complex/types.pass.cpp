// AST-transferred from libc++ by tools/transfer.py (slug=numerics_complex_number_complex_types).
// main -> test_numerics_complex_number_complex_types; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <complex>

// template<class T>
// class complex
// {
// public:
//   typedef T value_type;
//   ...
// };

#include <complex>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_numerics_complex_number_complex_types { // libcis: isolate file-scope helpers
template <class T>
void
test()
{
    typedef std::complex<T> C;
    static_assert((std::is_same<typename C::value_type, T>::value), "");
}
} using namespace libcis_ns_numerics_complex_number_complex_types; // libcis


void test_numerics_complex_number_complex_types()
{
    test<float>();
    test<double>();
    test<long double>();

  return;
}
