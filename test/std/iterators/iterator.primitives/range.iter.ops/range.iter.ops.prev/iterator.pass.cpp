// transferred+adapted from libc++ by tools/transfer.py (slug=iterators_iterator_primitives_range_iter_ops_range_iter_ops_prev_iterator).
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

#include "test_iterators.h"

namespace libcis_ns_iterators_iterator_primitives_range_iter_ops_range_iter_ops_prev_iterator_8ff4bfc2 { // libcis
template <class It>
constexpr void check(int* first, int* expected) {
  It it(first);
  auto result = std::ranges::prev(std::move(it)); // libcis: gcc-10 rejects constrained placeholder with dependent constraint arg here
  static_assert(std::is_same_v<decltype(result), It>);
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


int main(int, char**) {
  test();
  static_assert(test());
  return 0;
}
} // libcis_ns_iterators_iterator_primitives_range_iter_ops_range_iter_ops_prev_iterator_8ff4bfc2 (libcis)

