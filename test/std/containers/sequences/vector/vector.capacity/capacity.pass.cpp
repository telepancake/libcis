// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_vector_vector_capacity_capacity).
// main -> test_containers_sequences_vector_vector_capacity_capacity; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <vector>

// size_type capacity() const;

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "asan_testing.h"

namespace libcis_ns_containers_sequences_vector_vector_capacity_capacity { // libcis: isolate file-scope helpers
TEST_CONSTEXPR_CXX20 bool tests() {
  {
    std::vector<int> v;
    assert(v.capacity() == 0);
    assert(is_contiguous_container_asan_correct(v));
  }
  {
    std::vector<int> v(100);
    assert(v.capacity() == 100);
    v.push_back(0);
    assert(v.capacity() > 101);
    assert(is_contiguous_container_asan_correct(v));
  }
#if TEST_STD_VER >= 11
  {
    std::vector<int, min_allocator<int>> v;
    assert(v.capacity() == 0);
    assert(is_contiguous_container_asan_correct(v));
  }
  {
    std::vector<int, min_allocator<int>> v(100);
    assert(v.capacity() == 100);
    v.push_back(0);
    assert(v.capacity() > 101);
    assert(is_contiguous_container_asan_correct(v));
  }
#endif

  return true;
}
} using namespace libcis_ns_containers_sequences_vector_vector_capacity_capacity; // libcis


void test_containers_sequences_vector_vector_capacity_capacity() {
  tests();
#if TEST_STD_VER > 17
  static_assert(tests());
#endif
  return;
}
