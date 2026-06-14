// transferred+adapted from libc++ by tools/transfer.py (slug=iterators_iterator_primitives_range_iter_ops_range_iter_ops_prev_iterator_count_sentinel).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// ranges::prev(it, n, bound)

#include <iterator>

#include <cassert>
#include <concepts>
#include <type_traits> // libcis
#include <utility>

#include "test_iterators.h"

namespace libcis_ns_iterators_iterator_primitives_range_iter_ops_range_iter_ops_prev_iterator_count_sentinel { // libcis
template <typename It>
constexpr void check(int* first, int* last, std::iter_difference_t<It> n, int* expected) {
  It it(last);
  It sent(first); // for std::ranges::prev, the sentinel *must* have the same type as the iterator

  auto result = std::ranges::prev(std::move(it), n, std::move(sent)); // libcis: gcc-10 rejects constrained placeholder with dependent constraint arg here
  static_assert(std::is_same_v<decltype(result), It>);
  assert(base(result) == expected);
}

constexpr bool test() {
  int range[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  for (int size = 0; size != 10; ++size) {
    for (int n = 0; n != 20; ++n) {
      int* expected = n > size ? range : range + size - n;
      check<bidirectional_iterator<int*>>(range, range+size, n, expected);
      check<random_access_iterator<int*>>(range, range+size, n, expected);
      check<contiguous_iterator<int*>>(   range, range+size, n, expected);
      check<int*>(                        range, range+size, n, expected);
    }
  }

  return true;
}

int main(int, char**) {
  assert(test());
  static_assert(test());
  return 0;
}
} // libcis_ns_iterators_iterator_primitives_range_iter_ops_range_iter_ops_prev_iterator_count_sentinel (libcis)

