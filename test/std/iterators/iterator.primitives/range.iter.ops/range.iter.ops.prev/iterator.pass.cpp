// AST-transferred from libc++ by tools/transfer.py (slug=iterators_iterator_primitives_range_iter_ops_range_iter_ops_prev_iterator).
// main -> test_iterators_iterator_primitives_range_iter_ops_range_iter_ops_prev_iterator; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// ranges::prev(it)

#include <iterator>
#include <cassert>
#include <utility>

#include "test_iterators.h"

namespace libcis_ns_iterators_iterator_primitives_range_iter_ops_range_iter_ops_prev_iterator { // libcis: isolate file-scope helpers
template <class It>
constexpr void check(int* first, int* expected) {
  It it(first);
  std::same_as<It> auto result = std::ranges::prev(std::move(it));
  assert(base(result) == expected);
}

constexpr bool test() {
  int range[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  for (int n = 1; n != 10; ++n) {
    check<bidirectional_iterator<int*>>(range+n, range+n-1);
    check<random_access_iterator<int*>>(range+n, range+n-1);
    check<contiguous_iterator<int*>>(   range+n, range+n-1);
    check<int*>(                        range+n, range+n-1);
  }

  return true;
}
} using namespace libcis_ns_iterators_iterator_primitives_range_iter_ops_range_iter_ops_prev_iterator; // libcis



void test_iterators_iterator_primitives_range_iter_ops_range_iter_ops_prev_iterator() {
  test();
  static_assert(test());
  return;
}
