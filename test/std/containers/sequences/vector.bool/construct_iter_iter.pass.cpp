// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_vector_bool_construct_iter_iter).
// main -> test_containers_sequences_vector_bool_construct_iter_iter; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <vector>
// vector<bool>

// template <class InputIter> vector(InputIter first, InputIter last);

#include <algorithm>
#include <vector>
#include <cassert>
#include <cstddef>

#include "test_macros.h"
#include "test_iterators.h"
#include "min_allocator.h"

namespace libcis_ns_containers_sequences_vector_bool_construct_iter_iter { // libcis: isolate file-scope helpers
template <class C, class Iterator>
TEST_CONSTEXPR_CXX20 void test(Iterator first, Iterator last) {
  C c(first, last);
  LIBCPP_ASSERT(c.__invariants());
  assert(c.size() == static_cast<std::size_t>(std::distance(first, last)));
  assert(std::equal(c.cbegin(), c.cend(), first));
}

TEST_CONSTEXPR_CXX20 bool tests() {
  bool a[] = {0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0};
  bool* an = a + sizeof(a) / sizeof(a[0]);
  test<std::vector<bool> >(cpp17_input_iterator<const bool*>(a), cpp17_input_iterator<const bool*>(an));
  test<std::vector<bool> >(forward_iterator<const bool*>(a), forward_iterator<const bool*>(an));
  test<std::vector<bool> >(bidirectional_iterator<const bool*>(a), bidirectional_iterator<const bool*>(an));
  test<std::vector<bool> >(random_access_iterator<const bool*>(a), random_access_iterator<const bool*>(an));
  test<std::vector<bool> >(a, an);
#if TEST_STD_VER >= 11
  test<std::vector<bool, min_allocator<bool>> >(
      cpp17_input_iterator<const bool*>(a), cpp17_input_iterator<const bool*>(an));
  test<std::vector<bool, min_allocator<bool>> >(forward_iterator<const bool*>(a), forward_iterator<const bool*>(an));
  test<std::vector<bool, min_allocator<bool>> >(
      bidirectional_iterator<const bool*>(a), bidirectional_iterator<const bool*>(an));
  test<std::vector<bool, min_allocator<bool>> >(
      random_access_iterator<const bool*>(a), random_access_iterator<const bool*>(an));
  test<std::vector<bool, min_allocator<bool>> >(a, an);
#endif

  return true;
}
} using namespace libcis_ns_containers_sequences_vector_bool_construct_iter_iter; // libcis


void test_containers_sequences_vector_bool_construct_iter_iter() {
  tests();
#if TEST_STD_VER > 17
  static_assert(tests());
#endif
  return;
}
