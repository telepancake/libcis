// AST-transferred from libc++ by tools/transfer.py (slug=atomics_atomics_types_generic_atomics_types_float_types).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: c++03, c++11, c++14, c++17

//    using value_type = floating-point-type;
//    using difference_type = value_type;
// The atomic floating-point specializations are standard-layout structs. They each have a trivial destructor.

#include <atomic>
#include <type_traits>

namespace libcis_ns_atomics_atomics_types_generic_atomics_types_float_types { // libcis: isolate file-scope helpers
template <class T>
void test() {
  // LWG 3045. atomic<floating-point> doesn't have value_type or difference_type
  // https://cplusplus.github.io/LWG/issue3045
  static_assert(std::is_same_v<typename std::atomic<T>::value_type, T>);
  static_assert(std::is_same_v<typename std::atomic<T>::difference_type, T>);

  static_assert(std::is_standard_layout_v<std::atomic<T>>);
  static_assert(std::is_trivially_destructible_v<std::atomic<T>>);
}
} using namespace libcis_ns_atomics_atomics_types_generic_atomics_types_float_types; // libcis


template void test<float>();
template void test<double>();
template void test<long double>();
