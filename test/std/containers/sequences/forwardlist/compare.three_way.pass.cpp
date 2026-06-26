// transferred+adapted from libc++ by tools/transfer.py (slug=containers_sequences_forwardlist_compare_three_way_725c9fa1).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <forward_list>

// template<class T, class Allocator>
//   synth-three-way-result<T> operator<=>(const forward_list<T, Allocator>& x,
//                                         const forward_list<T, Allocator>& y);

#include <cassert>
#include <forward_list>

#include "test_container_comparisons.h"

namespace libcis_ns_containers_sequences_forwardlist_compare_three_way_725c9fa1 { // libcis
int main(int, char**) {
  assert(test_sequence_container_spaceship<std::forward_list>());
  // `std::forward_list` is not constexpr, so no `static_assert` test here.
  return 0;

    return 0;
}
} // libcis_ns_containers_sequences_forwardlist_compare_three_way_725c9fa1 (libcis)

