// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_deque_deque_cons_copy_alloc).
// main -> test_containers_sequences_deque_deque_cons_copy_alloc; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <deque>

// deque(const deque& c, const allocator_type& a);

#include "asan_testing.h"
#include <deque>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"
#include "min_allocator.h"

namespace libcis_ns_containers_sequences_deque_deque_cons_copy_alloc { // libcis: isolate file-scope helpers
template <class C>
void test(const C& x, const typename C::allocator_type& a) {
  C c(x, a);
  assert(c == x);
  assert(c.get_allocator() == a);
  LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c));
}
} using namespace libcis_ns_containers_sequences_deque_deque_cons_copy_alloc; // libcis


void test_containers_sequences_deque_deque_cons_copy_alloc() {
  {
    int ab[] = {3, 4, 2, 8, 0, 1, 44, 34, 45, 96, 80, 1, 13, 31, 45};
    int* an  = ab + sizeof(ab) / sizeof(ab[0]);
    test(std::deque<int, test_allocator<int> >(ab, an, test_allocator<int>(3)), test_allocator<int>(4));
  }
  {
    int ab[] = {3, 4, 2, 8, 0, 1, 44, 34, 45, 96, 80, 1, 13, 31, 45};
    int* an  = ab + sizeof(ab) / sizeof(ab[0]);
    test(std::deque<int, other_allocator<int> >(ab, an, other_allocator<int>(3)), other_allocator<int>(4));
  }
#if TEST_STD_VER >= 11
  {
    int ab[] = {3, 4, 2, 8, 0, 1, 44, 34, 45, 96, 80, 1, 13, 31, 45};
    int* an  = ab + sizeof(ab) / sizeof(ab[0]);
    test(std::deque<int, min_allocator<int> >(ab, an, min_allocator<int>()), min_allocator<int>());
  }
  {
    int ab[] = {3, 4, 2, 8, 0, 1, 44, 34, 45, 96, 80, 1, 13, 31, 45};
    int* an  = ab + sizeof(ab) / sizeof(ab[0]);
    test(std::deque<int, safe_allocator<int> >(ab, an, safe_allocator<int>()), safe_allocator<int>());
  }
#endif

  return;
}
