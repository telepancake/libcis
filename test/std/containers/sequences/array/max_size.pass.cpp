// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_array_max_size).
// main -> test_containers_sequences_array_max_size; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <array>

// class array

// constexpr bool max_size() const noexcept;

#include <array>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_containers_sequences_array_max_size { // libcis: isolate file-scope helpers
TEST_CONSTEXPR_CXX14 bool tests() {
  {
    typedef std::array<int, 2> C;
    C c = {};
    ASSERT_NOEXCEPT(c.max_size());
    assert(c.max_size() == 2);
  }
  {
    typedef std::array<int, 0> C;
    C c = {};
    ASSERT_NOEXCEPT(c.max_size());
    assert(c.max_size() == 0);
  }

  return true;
}
} using namespace libcis_ns_containers_sequences_array_max_size; // libcis


void test_containers_sequences_array_max_size() {
  tests();
#if TEST_STD_VER >= 14
  static_assert(tests(), "");
#endif

#if TEST_STD_VER >= 11
  // Sanity check for constexpr in C++11
  {
    constexpr std::array<int, 3> array = {};
    static_assert(array.max_size() == 3, "");
  }
#endif

  return;
}
