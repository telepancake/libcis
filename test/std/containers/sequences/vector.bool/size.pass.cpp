// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_vector_bool_size).
// main -> test_containers_sequences_vector_bool_size; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <vector>

// class vector

// size_type size() const noexcept;

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"

namespace libcis_ns_containers_sequences_vector_bool_size { // libcis: isolate file-scope helpers
TEST_CONSTEXPR_CXX20 bool tests() {
  {
    typedef std::vector<bool> C;
    C c;
    ASSERT_NOEXCEPT(c.size());
    assert(c.size() == 0);
    c.push_back(false);
    assert(c.size() == 1);
    c.push_back(true);
    assert(c.size() == 2);
    c.push_back(false);
    assert(c.size() == 3);
    c.erase(c.begin());
    assert(c.size() == 2);
    c.erase(c.begin());
    assert(c.size() == 1);
    c.erase(c.begin());
    assert(c.size() == 0);
  }
#if TEST_STD_VER >= 11
  {
    typedef std::vector<bool, min_allocator<bool>> C;
    C c;
    ASSERT_NOEXCEPT(c.size());
    assert(c.size() == 0);
    c.push_back(false);
    assert(c.size() == 1);
    c.push_back(true);
    assert(c.size() == 2);
    c.push_back(false);
    assert(c.size() == 3);
    c.erase(c.begin());
    assert(c.size() == 2);
    c.erase(c.begin());
    assert(c.size() == 1);
    c.erase(c.begin());
    assert(c.size() == 0);
  }
#endif

  return true;
}
} using namespace libcis_ns_containers_sequences_vector_bool_size; // libcis


void test_containers_sequences_vector_bool_size() {
  tests();
#if TEST_STD_VER > 17
  static_assert(tests());
#endif
  return;
}
