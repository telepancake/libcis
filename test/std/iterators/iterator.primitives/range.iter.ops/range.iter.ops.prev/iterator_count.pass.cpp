// transferred+adapted from libc++ by tools/transfer.py (slug=iterators_iterator_primitives_range_iter_ops_range_iter_ops_prev_iterator_count_91f52f9b).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// ranges::prev(it, n)

#include <iterator>

#include <cassert>
#include <concepts>
#include <utility>

#include "test_iterators.h"

namespace libcis_ns_iterators_iterator_primitives_range_iter_ops_range_iter_ops_prev_iterator_count_91f52f9b { // libcis
template <typename It>
constexpr void check(int* first, std::iter_difference_t<It> n, int* expected) {
  It it(first);
  std::same_as<It> auto result = std::ranges::prev(std::move(it), n);
  assert(base(result) == expected);
}

constexpr bool test() {
  int range[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  // Check prev() forward
  for (int n = 0; n != 10; ++n) {
    check<bidirectional_iterator<int*>>(range+n, n, range);
    check<random_access_iterator<int*>>(range+n, n, range);
    check<contiguous_iterator<int*>>(   range+n, n, range);
    check<int*>(                        range+n, n, range);
  }

  // Check prev() backward
  for (int n = 0; n != 10; ++n) {
    check<bidirectional_iterator<int*>>(range, -n, range+n);
    check<random_access_iterator<int*>>(range, -n, range+n);
    check<contiguous_iterator<int*>>(   range, -n, range+n);
    check<int*>(                        range, -n, range+n);
  }

  return true;
}

int main(int, char**) {
  test();
  static_assert(test());
  return 0;

    return 0;
}
} // libcis_ns_iterators_iterator_primitives_range_iter_ops_range_iter_ops_prev_iterator_count_91f52f9b (libcis)

