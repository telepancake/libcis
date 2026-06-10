// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_vector_vector_cons_initializer_list).
// main -> test_containers_sequences_vector_vector_cons_initializer_list; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <vector>

// vector(initializer_list<value_type> il);

#include <vector>
#include <cassert>
#include "test_macros.h"
#include "min_allocator.h"
#include "asan_testing.h"

namespace libcis_ns_containers_sequences_vector_vector_cons_initializer_list { // libcis: isolate file-scope helpers
TEST_CONSTEXPR_CXX20 bool tests() {
  {
    std::vector<int> d = {3, 4, 5, 6};
    assert(d.size() == 4);
    assert(is_contiguous_container_asan_correct(d));
    assert(d[0] == 3);
    assert(d[1] == 4);
    assert(d[2] == 5);
    assert(d[3] == 6);
  }
  {
    std::vector<int, min_allocator<int>> d = {3, 4, 5, 6};
    assert(d.size() == 4);
    assert(is_contiguous_container_asan_correct(d));
    assert(d[0] == 3);
    assert(d[1] == 4);
    assert(d[2] == 5);
    assert(d[3] == 6);
  }
  {
    std::vector<int, safe_allocator<int>> d = {3, 4, 5, 6};
    assert(d.size() == 4);
    assert(is_contiguous_container_asan_correct(d));
    assert(d[0] == 3);
    assert(d[1] == 4);
    assert(d[2] == 5);
    assert(d[3] == 6);
  }

  return true;
}
} using namespace libcis_ns_containers_sequences_vector_vector_cons_initializer_list; // libcis


void test_containers_sequences_vector_vector_cons_initializer_list() {
  tests();
#if TEST_STD_VER > 17
  static_assert(tests());
#endif
  return;
}
