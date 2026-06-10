// AST-transferred from libc++ by tools/transfer.py (slug=containers_unord_unord_multiset_unord_multiset_cnstr_from_range).
// main -> test_containers_unord_unord_multiset_unord_multiset_cnstr_from_range; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// template<container-compatible-range<value_type> R>
//   unordered_multiset(from_range_t, R&& rg, size_type n = see below,
//     const hasher& hf = hasher(), const key_equal& eql = key_equal(),
//     const allocator_type& a = allocator_type()); // C++23
//
// template<container-compatible-range<value_type> R>
//   unordered_multiset(from_range_t, R&& rg, size_type n, const allocator_type& a)
//     : unordered_multiset(from_range, std::forward<R>(rg), n, hasher(), key_equal(), a) { } // C++23
//
// template<container-compatible-range<value_type> R>
//   unordered_multiset(from_range_t, R&& rg, size_type n, const hasher& hf, const allocator_type& a)
//     : unordered_multiset(from_range, std::forward<R>(rg), n, hf, key_equal(), a) { }       // C++23

#include <array>
#include <unordered_set>

#include "../../from_range_unordered_containers.h"
#include "test_macros.h"

namespace libcis_ns_containers_unord_unord_multiset_unord_multiset_cnstr_from_range { // libcis: isolate file-scope helpers
void test_duplicates() {
  std::array input = {1, 2, 3, 3, 3, 4, 2, 1, 2};
  auto c           = std::unordered_multiset<int>(std::from_range, input);
  assert(std::ranges::is_permutation(input, c));
}
} using namespace libcis_ns_containers_unord_unord_multiset_unord_multiset_cnstr_from_range; // libcis


void test_containers_unord_unord_multiset_unord_multiset_cnstr_from_range() {
  for_all_iterators_and_allocators<int>([]<class Iter, class Sent, class Alloc>() {
    test_unordered_set<std::unordered_multiset, int, Iter, Sent, test_hash<int>, test_equal_to<int>, Alloc>();
  });
  test_unordered_set_move_only<std::unordered_multiset>();

  static_assert(test_set_constraints<std::unordered_set, int, double>());

  test_set_exception_safety_throwing_copy<std::unordered_multiset>();
  test_set_exception_safety_throwing_allocator<std::unordered_multiset, int>();

  return;
}
