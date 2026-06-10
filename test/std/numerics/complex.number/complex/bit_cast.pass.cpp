// AST-transferred from libc++ by tools/transfer.py (slug=numerics_complex_number_complex_bit_cast).
// main -> test_numerics_complex_number_complex_bit_cast; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// Make sure that std::bit_cast works with std::complex. Test case extracted from https://llvm.org/PR94620.

#include <bit>
#include <complex>

namespace libcis_ns_numerics_complex_number_complex_bit_cast { // libcis: isolate file-scope helpers
template <class T>
constexpr void test() {
  using Complex                       = std::complex<T>;
  unsigned char data[sizeof(Complex)] = {0};

  [[maybe_unused]] Complex c = std::bit_cast<Complex>(data);
}

constexpr bool test_all() {
  test<float>();
  test<double>();
  test<long double>();
  return true;
}
} using namespace libcis_ns_numerics_complex_number_complex_bit_cast; // libcis


void test_numerics_complex_number_complex_bit_cast() {
  test_all();
  static_assert(test_all());
  return;
}
