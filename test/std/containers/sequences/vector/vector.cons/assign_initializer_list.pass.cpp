// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_vector_vector_cons_assign_initializer_list).
// main -> test_containers_sequences_vector_vector_cons_assign_initializer_list; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <vector>

// void assign(initializer_list<value_type> il);

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "asan_testing.h"

namespace libcis_ns_containers_sequences_vector_vector_cons_assign_initializer_list { // libcis: isolate file-scope helpers
template <typename Vec>
TEST_CONSTEXPR_CXX20 void test(Vec& v) {
  v.assign({3, 4, 5, 6});
  assert(v.size() == 4);
  assert(is_contiguous_container_asan_correct(v));
  assert(v[0] == 3);
  assert(v[1] == 4);
  assert(v[2] == 5);
  assert(v[3] == 6);
}

TEST_CONSTEXPR_CXX20 bool tests() {
  {
    typedef std::vector<int> V;
    V d1;
    V d2;
    d2.reserve(10); // no reallocation during assign.
    test(d1);
    test(d2);
  }
  {
    typedef std::vector<int, min_allocator<int>> V;
    V d1;
    V d2;
    d2.reserve(10); // no reallocation during assign.
    test(d1);
    test(d2);
  }

  return true;
}
} using namespace libcis_ns_containers_sequences_vector_vector_cons_assign_initializer_list; // libcis


void test_containers_sequences_vector_vector_cons_assign_initializer_list() {
  tests();
#if TEST_STD_VER > 17
  static_assert(tests());
#endif
  return;
}
