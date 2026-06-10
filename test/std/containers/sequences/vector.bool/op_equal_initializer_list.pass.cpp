// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_vector_bool_op_equal_initializer_list).
// main -> test_containers_sequences_vector_bool_op_equal_initializer_list; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <vector>

// vector& operator=(initializer_list<value_type> il);

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"

namespace libcis_ns_containers_sequences_vector_bool_op_equal_initializer_list { // libcis: isolate file-scope helpers
TEST_CONSTEXPR_CXX20 bool tests() {
  {
    std::vector<bool> d;
    d = {true, false, false, true};
    assert(d.size() == 4);
    assert(d[0] == true);
    assert(d[1] == false);
    assert(d[2] == false);
    assert(d[3] == true);
  }
  {
    std::vector<bool, min_allocator<bool>> d;
    d = {true, false, false, true};
    assert(d.size() == 4);
    assert(d[0] == true);
    assert(d[1] == false);
    assert(d[2] == false);
    assert(d[3] == true);
  }

  return true;
}
} using namespace libcis_ns_containers_sequences_vector_bool_op_equal_initializer_list; // libcis


void test_containers_sequences_vector_bool_op_equal_initializer_list() {
  tests();
#if TEST_STD_VER > 17
  static_assert(tests());
#endif
  return;
}
