// transferred+adapted from libc++ by tools/transfer.py (slug=iterators_iterator_primitives_range_iter_ops_range_iter_ops_prev_constraints_f1d674a9).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// ranges::prev
// Make sure we're SFINAE-friendly when the template argument constraints are not met.

#include <iterator>

#include <cstddef>
#include <utility>
#include "test_iterators.h"

namespace libcis_ns_iterators_iterator_primitives_range_iter_ops_range_iter_ops_prev_constraints_f1d674a9 { // libcis
template <class ...Args>
concept has_ranges_prev = requires (Args&& ...args) {
  { std::ranges::prev(std::forward<Args>(args)...) };
};

using It = forward_iterator<int*>;
static_assert(!has_ranges_prev<It>);
static_assert(!has_ranges_prev<It, std::ptrdiff_t>);
static_assert(!has_ranges_prev<It, std::ptrdiff_t, It>);

// Test the test
using It2 = bidirectional_iterator<int*>;
static_assert(has_ranges_prev<It2>);
static_assert(has_ranges_prev<It2, std::ptrdiff_t>);
static_assert(has_ranges_prev<It2, std::ptrdiff_t, It2>);
} // libcis_ns_iterators_iterator_primitives_range_iter_ops_range_iter_ops_prev_constraints_f1d674a9 (libcis)

