// AST-transferred from libc++ by tools/transfer.py (slug=iterators_predef_iterators_iterators_common_ctor_iter).
// main -> test_iterators_predef_iterators_iterators_common_ctor_iter; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr common_iterator(I i);

#include <iterator>
#include <cassert>
#include <utility>

#include "test_iterators.h"

namespace libcis_ns_iterators_predef_iterators_iterators_common_ctor_iter { // libcis: isolate file-scope helpers
template<class It>
constexpr bool test() {
  using CommonIt = std::common_iterator<It, sentinel_wrapper<It>>;
  int a[] = {1,2,3};
  It it = It(a);
  CommonIt lv = CommonIt(it);
  assert(&*lv == a);
  CommonIt rv = CommonIt(std::move(it));
  assert(&*rv == a);

  return true;
}
} using namespace libcis_ns_iterators_predef_iterators_iterators_common_ctor_iter; // libcis


void test_iterators_predef_iterators_iterators_common_ctor_iter() {
  test<cpp17_input_iterator<int*>>();
  test<forward_iterator<int*>>();
  test<bidirectional_iterator<int*>>();
  test<random_access_iterator<int*>>();
  test<contiguous_iterator<int*>>();
  test<int*>();
  test<const int*>();

  static_assert(test<cpp17_input_iterator<int*>>());
  static_assert(test<forward_iterator<int*>>());
  static_assert(test<bidirectional_iterator<int*>>());
  static_assert(test<random_access_iterator<int*>>());
  static_assert(test<contiguous_iterator<int*>>());
  static_assert(test<int*>());
  static_assert(test<const int*>());

  return;
}
