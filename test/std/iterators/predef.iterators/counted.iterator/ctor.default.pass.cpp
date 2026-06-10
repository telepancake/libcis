// AST-transferred from libc++ by tools/transfer.py (slug=iterators_predef_iterators_counted_iterator_ctor_default).
// main -> test_iterators_predef_iterators_counted_iterator_ctor_default; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr counted_iterator() requires default_initializable<I> = default;

#include <cassert>
#include <iterator>

#include "test_macros.h"
#include "test_iterators.h"

namespace libcis_ns_iterators_predef_iterators_counted_iterator_ctor_default { // libcis: isolate file-scope helpers
constexpr bool test() {
  static_assert(!std::default_initializable<std::counted_iterator<cpp17_input_iterator<int*>>>);
  static_assert( std::default_initializable<std::counted_iterator<forward_iterator<int*>>>);

  std::counted_iterator<forward_iterator<int*>> iter;
  assert(iter.base() == forward_iterator<int*>());
  assert(iter.count() == 0);

  return true;
}
} using namespace libcis_ns_iterators_predef_iterators_counted_iterator_ctor_default; // libcis


void test_iterators_predef_iterators_counted_iterator_ctor_default() {
  test();
  static_assert(test());

  return;
}
