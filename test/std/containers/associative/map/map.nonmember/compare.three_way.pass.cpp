// transferred+adapted from libc++ by tools/transfer.py (slug=containers_associative_map_map_nonmember_compare_three_way_6cff4c6a).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <map>

// class map

// template<class Key, class T, class Compare, class Allocator>
//   synth-three-way-result<pair<const Key, T>>
//     operator<=>(const map<Key, T, Compare, Allocator>& x,
//                 const map<Key, T, Compare, Allocator>& y);

#include <cassert>
#include <map>

#include "test_container_comparisons.h"

namespace libcis_ns_containers_associative_map_map_nonmember_compare_three_way_6cff4c6a { // libcis
int main(int, char**) {
  assert(test_ordered_map_container_spaceship<std::map>());
  // `std::map` is not constexpr, so no `static_assert` test here.
  return 0;

    return 0;
}
} // libcis_ns_containers_associative_map_map_nonmember_compare_three_way_6cff4c6a (libcis)

