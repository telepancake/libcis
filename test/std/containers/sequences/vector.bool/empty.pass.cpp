// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_vector_bool_empty).
// main -> test_containers_sequences_vector_bool_empty; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <vector>

// class vector

// bool empty() const noexcept;

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"

namespace libcis_ns_containers_sequences_vector_bool_empty { // libcis: isolate file-scope helpers
TEST_CONSTEXPR_CXX20 bool tests() {
  {
    typedef std::vector<bool> C;
    C c;
    ASSERT_NOEXCEPT(c.empty());
    assert(c.empty());
    c.push_back(false);
    assert(!c.empty());
    c.clear();
    assert(c.empty());
  }
#if TEST_STD_VER >= 11
  {
    typedef std::vector<bool, min_allocator<bool>> C;
    C c;
    ASSERT_NOEXCEPT(c.empty());
    assert(c.empty());
    c.push_back(false);
    assert(!c.empty());
    c.clear();
    assert(c.empty());
  }
#endif

  return true;
}
} using namespace libcis_ns_containers_sequences_vector_bool_empty; // libcis


void test_containers_sequences_vector_bool_empty() {
  tests();
#if TEST_STD_VER > 17
  static_assert(tests());
#endif
  return;
}
