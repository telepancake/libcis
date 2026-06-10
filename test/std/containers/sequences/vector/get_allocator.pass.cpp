// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_vector_get_allocator).
// main -> test_containers_sequences_vector_get_allocator; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <vector>

// class vector

// allocator_type get_allocator() const

#include <vector>
#include <cassert>

#include "test_allocator.h"
#include "test_macros.h"

namespace libcis_ns_containers_sequences_vector_get_allocator { // libcis: isolate file-scope helpers
TEST_CONSTEXPR_CXX20 bool test() {
  {
    std::allocator<int> alloc;
    const std::vector<int> v(alloc);
    assert(v.get_allocator() == alloc);
  }
  {
    other_allocator<int> alloc(1);
    const std::vector<int, other_allocator<int> > v(alloc);
    assert(v.get_allocator() == alloc);
  }

  return true;
}
} using namespace libcis_ns_containers_sequences_vector_get_allocator; // libcis


void test_containers_sequences_vector_get_allocator() {
  test();
#if TEST_STD_VER > 17
  static_assert(test());
#endif

  return;
}
