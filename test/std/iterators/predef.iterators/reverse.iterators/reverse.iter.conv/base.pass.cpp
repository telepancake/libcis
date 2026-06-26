// transferred+adapted from libc++ by tools/transfer.py (slug=iterators_predef_iterators_reverse_iterators_reverse_iter_conv_base_3828edd2).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iterator>

// reverse_iterator

// iterator_type base() const; // constexpr since C++17

#include <iterator>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"

namespace libcis_ns_iterators_predef_iterators_reverse_iterators_reverse_iter_conv_base_3828edd2 { // libcis
template <class Iter>
TEST_CONSTEXPR_CXX17 void test() {
  int i = 0;
  Iter iter(&i);
  std::reverse_iterator<Iter> const reverse(iter);
  typename std::reverse_iterator<Iter>::iterator_type base = reverse.base();
  assert(base == Iter(&i));
}

TEST_CONSTEXPR_CXX17 bool tests() {
  test<bidirectional_iterator<int*> >();
  test<random_access_iterator<int*> >();
#if TEST_STD_VER >= 20
  test<cpp20_random_access_iterator<int*>>();
#endif
  return true;
}

int main(int, char**) {
  tests();
#if TEST_STD_VER > 14
  static_assert(tests(), "");
#endif
  return 0;

    return 0;
}
} // libcis_ns_iterators_predef_iterators_reverse_iterators_reverse_iter_conv_base_3828edd2 (libcis)

