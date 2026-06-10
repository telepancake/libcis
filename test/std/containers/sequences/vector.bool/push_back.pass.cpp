// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_vector_bool_push_back).
// main -> test_containers_sequences_vector_bool_push_back; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <vector>
// vector<bool>

// void push_back(const value_type& x);

#include <vector>
#include <cassert>
#include <cstddef>

#include "test_macros.h"
#include "min_allocator.h"

namespace libcis_ns_containers_sequences_vector_bool_push_back { // libcis: isolate file-scope helpers
TEST_CONSTEXPR_CXX20 bool tests() {
  {
    bool a[]         = {0, 1, 1, 0, 1, 0, 0};
    const unsigned N = sizeof(a) / sizeof(a[0]);
    std::vector<bool> c;
    for (unsigned i = 0; i < N; ++i) {
      c.push_back(a[i]);
      assert(c.size() == i + 1);
      for (std::size_t j = 0; j < c.size(); ++j)
        assert(c[j] == a[j]);
    }
  }
#if TEST_STD_VER >= 11
  {
    bool a[]         = {0, 1, 1, 0, 1, 0, 0};
    const unsigned N = sizeof(a) / sizeof(a[0]);
    std::vector<bool, min_allocator<bool>> c;
    for (unsigned i = 0; i < N; ++i) {
      c.push_back(a[i]);
      assert(c.size() == i + 1);
      for (std::size_t j = 0; j < c.size(); ++j)
        assert(c[j] == a[j]);
    }
  }
#endif

  return true;
}
} using namespace libcis_ns_containers_sequences_vector_bool_push_back; // libcis


void test_containers_sequences_vector_bool_push_back() {
  tests();
#if TEST_STD_VER > 17
  static_assert(tests());
#endif
  return;
}
