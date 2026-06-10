// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_array_array_tuple_tuple_size).
// main -> test_containers_sequences_array_array_tuple_tuple_size; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <array>

// tuple_size<array<T, N> >::value

#include <array>

#include "test_macros.h"

namespace libcis_ns_containers_sequences_array_array_tuple_tuple_size { // libcis: isolate file-scope helpers
template <class T, std::size_t N>
void test() {
  {
    typedef std::array<T, N> C;
    static_assert((std::tuple_size<C>::value == N), "");
  }
  {
    typedef std::array<T const, N> C;
    static_assert((std::tuple_size<C>::value == N), "");
  }
  {
    typedef std::array<T volatile, N> C;
    static_assert((std::tuple_size<C>::value == N), "");
  }
  {
    typedef std::array<T const volatile, N> C;
    static_assert((std::tuple_size<C>::value == N), "");
  }
}
} using namespace libcis_ns_containers_sequences_array_array_tuple_tuple_size; // libcis


void test_containers_sequences_array_array_tuple_tuple_size() {
  test<double, 0>();
  test<double, 3>();
  test<double, 5>();

  return;
}
