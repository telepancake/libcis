// AST-transferred from libc++ by tools/transfer.py (slug=numerics_numarray_template_valarray_valarray_cassign_xor_valarray).
// main -> test_numerics_numarray_template_valarray_valarray_cassign_xor_valarray; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <valarray>

// template<class T> class valarray;

// [valarray.syn]/3
//   Any function returning a valarray<T> is permitted to return an object of
//   another type, provided all the const member functions of valarray<T> are
//   also applicable to this type.
//
// Libc++ uses this and returns __val_expr<_Expr> for several operations.
//
// The const overloads of
//   valarray::operator[](...) const
// return proxy objects. These proxies are implicitly convertible to
// std::valarray.
//
// valarray& operator^=(const valarray& v);

#include <valarray>
#include <cassert>
#include <cstddef>

#include "test_macros.h"

namespace libcis_ns_numerics_numarray_template_valarray_valarray_cassign_xor_valarray { // libcis: isolate file-scope helpers
template <class A>
void test(const A& rhs) {
  int input[]      = {1, 2, 3, 4, 5};
  int expected[]   = {7, 5, 11, 13, 15};
  const unsigned N = sizeof(input) / sizeof(input[0]);
  std::valarray<int> value(input, N);

  value ^= rhs;

  assert(value.size() == N);
  for (std::size_t i = 0; i < value.size(); ++i)
    assert(value[i] == expected[i]);
}
} using namespace libcis_ns_numerics_numarray_template_valarray_valarray_cassign_xor_valarray; // libcis


void test_numerics_numarray_template_valarray_valarray_cassign_xor_valarray() {
  int input[]      = {6, 7, 8, 9, 10};
  const unsigned N = sizeof(input) / sizeof(input[0]);

  std::valarray<bool> mask(true, N);
  std::size_t indices[] = {0, 1, 2, 3, 4};
  std::valarray<std::size_t> indirect(indices, N);

  std::valarray<int> zero(0, N);

  {
    std::valarray<int> value(input, N);

    test(value);
    test(value[std::slice(0, N, 1)]);
    test(value[std::gslice(0, std::valarray<std::size_t>(N, 1), std::valarray<std::size_t>(1, 1))]);
    test(value[mask]);
    test(value[indirect]);
    test(value + zero);
  }

  {
    const std::valarray<int> value(input, N);

    test(value);
    test(value[std::slice(0, N, 1)]);
    test(value[std::gslice(0, std::valarray<std::size_t>(N, 1), std::valarray<std::size_t>(1, 1))]);
    test(value[mask]);
    test(value[indirect]);
    test(value + zero);
  }

  return;
}
