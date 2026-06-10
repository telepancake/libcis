// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_vector_bool_initializer_list_alloc).
// main -> test_containers_sequences_vector_bool_initializer_list_alloc; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <vector>

// vector(initializer_list<value_type> il, const Allocator& a = allocator_type());

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"
#include "min_allocator.h"

namespace libcis_ns_containers_sequences_vector_bool_initializer_list_alloc { // libcis: isolate file-scope helpers
TEST_CONSTEXPR_CXX20 bool tests() {
  {
    std::vector<bool, test_allocator<bool>> d({true, false, false, true}, test_allocator<bool>(3));
    assert(d.get_allocator() == test_allocator<bool>(3));
    assert(d.size() == 4);
    assert(d[0] == true);
    assert(d[1] == false);
    assert(d[2] == false);
    assert(d[3] == true);
  }
  {
    std::vector<bool, min_allocator<bool>> d({true, false, false, true}, min_allocator<bool>());
    assert(d.get_allocator() == min_allocator<bool>());
    assert(d.size() == 4);
    assert(d[0] == true);
    assert(d[1] == false);
    assert(d[2] == false);
    assert(d[3] == true);
  }

  return true;
}
} using namespace libcis_ns_containers_sequences_vector_bool_initializer_list_alloc; // libcis


void test_containers_sequences_vector_bool_initializer_list_alloc() {
  tests();
#if TEST_STD_VER > 17
  static_assert(tests());
#endif
  return;
}
