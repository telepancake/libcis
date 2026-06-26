// transferred+adapted from libc++ by tools/transfer.py (slug=containers_sequences_list_compare_three_way_dfe6456e).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <list>

// template <class T, class Allocator> constexpr
//   synth-three-way-result<T>
//     operator<=>(const list<T, Allocator>& x, const list<T, Allocator>& y);

#include <list>
#include <cassert>

#include "test_container_comparisons.h"

namespace libcis_ns_containers_sequences_list_compare_three_way_dfe6456e { // libcis
int main(int, char**) {
  assert(test_sequence_container_spaceship<std::list>());
  // `std::list` is not constexpr, so no `static_assert` test here.
  return 0;

    return 0;
}
} // libcis_ns_containers_sequences_list_compare_three_way_dfe6456e (libcis)

