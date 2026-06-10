// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_vector_bool_flip).
// main -> test_containers_sequences_vector_bool_flip; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <vector>

// void flip();

#include <cassert>
#include <memory>
#include <vector>

#include "min_allocator.h"
#include "test_allocator.h"
#include "test_macros.h"

namespace libcis_ns_containers_sequences_vector_bool_flip { // libcis: isolate file-scope helpers
template <typename Allocator>
TEST_CONSTEXPR_CXX20 void test_vector_flip(std::size_t n, Allocator a) {
  std::vector<bool, Allocator> v(n, false, a);
  for (std::size_t i = 0; i < n; ++i)
    v[i] = i & 1;
  std::vector<bool, Allocator> original = v;
  v.flip();
  for (size_t i = 0; i < n; ++i)
    assert(v[i] == !original[i]);
  v.flip();
  assert(v == original);
}

TEST_CONSTEXPR_CXX20 bool tests() {
  // Test empty vectors
  test_vector_flip(0, std::allocator<bool>());
  test_vector_flip(0, min_allocator<bool>());
  test_vector_flip(0, test_allocator<bool>(5));

  // Test small vectors with different allocators
  test_vector_flip(3, std::allocator<bool>());
  test_vector_flip(3, min_allocator<bool>());
  test_vector_flip(3, test_allocator<bool>(5));

  // Test large vectors with different allocators
  test_vector_flip(1000, std::allocator<bool>());
  test_vector_flip(1000, min_allocator<bool>());
  test_vector_flip(1000, test_allocator<bool>(5));

  return true;
}
} using namespace libcis_ns_containers_sequences_vector_bool_flip; // libcis


void test_containers_sequences_vector_bool_flip() {
  tests();
#if TEST_STD_VER >= 20
  static_assert(tests());
#endif
  return;
}
