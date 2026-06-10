// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_vector_vector_modifiers_clear).
// main -> test_containers_sequences_vector_vector_modifiers_clear; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <vector>

// void clear() noexcept;

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "asan_testing.h"

namespace libcis_ns_containers_sequences_vector_vector_modifiers_clear { // libcis: isolate file-scope helpers
TEST_CONSTEXPR_CXX20 bool tests() {
  {
    int a[] = {1, 2, 3};
    std::vector<int> c(a, a + 3);
    ASSERT_NOEXCEPT(c.clear());
    c.clear();
    assert(c.empty());
    LIBCPP_ASSERT(c.__invariants());
    LIBCPP_ASSERT(is_contiguous_container_asan_correct(c));
  }
#if TEST_STD_VER >= 11
  {
    int a[] = {1, 2, 3};
    std::vector<int, min_allocator<int>> c(a, a + 3);
    ASSERT_NOEXCEPT(c.clear());
    c.clear();
    assert(c.empty());
    LIBCPP_ASSERT(c.__invariants());
    LIBCPP_ASSERT(is_contiguous_container_asan_correct(c));
  }
  {
    int a[] = {1, 2, 3};
    std::vector<int, safe_allocator<int>> c(a, a + 3);
    ASSERT_NOEXCEPT(c.clear());
    c.clear();
    assert(c.empty());
    LIBCPP_ASSERT(c.__invariants());
    LIBCPP_ASSERT(is_contiguous_container_asan_correct(c));
  }
#endif

  return true;
}
} using namespace libcis_ns_containers_sequences_vector_vector_modifiers_clear; // libcis


void test_containers_sequences_vector_vector_modifiers_clear() {
  tests();
#if TEST_STD_VER > 17
  static_assert(tests());
#endif
  return;
}
