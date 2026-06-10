// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_array_indexing).
// main -> test_containers_sequences_array_indexing; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <array>

// reference operator[](size_type); // constexpr in C++17
// Libc++ marks it as noexcept

#include <array>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_containers_sequences_array_indexing { // libcis: isolate file-scope helpers
TEST_CONSTEXPR_CXX17 bool tests() {
  {
    typedef double T;
    typedef std::array<T, 3> C;
    C c = {1, 2, 3.5};
    LIBCPP_ASSERT_NOEXCEPT(c[0]);
    ASSERT_SAME_TYPE(C::reference, decltype(c[0]));
    C::reference r1 = c[0];
    assert(r1 == 1);
    r1 = 5.5;
    assert(c.front() == 5.5);

    C::reference r2 = c[2];
    assert(r2 == 3.5);
    r2 = 7.5;
    assert(c.back() == 7.5);
  }

  // Test operator[] "works" on zero sized arrays
  {
    {
      typedef double T;
      typedef std::array<T, 0> C;
      C c = {};
      LIBCPP_ASSERT_NOEXCEPT(c[0]);
      ASSERT_SAME_TYPE(C::reference, decltype(c[0]));
      if (c.size() > (0)) { // always false
        C::reference r = c[0];
        (void)r;
      }
    }
    {
      typedef double T;
      typedef std::array<const T, 0> C;
      C c = {};
      LIBCPP_ASSERT_NOEXCEPT(c[0]);
      ASSERT_SAME_TYPE(C::reference, decltype(c[0]));
      if (c.size() > (0)) { // always false
        C::reference r = c[0];
        (void)r;
      }
    }
  }

  return true;
}
} using namespace libcis_ns_containers_sequences_array_indexing; // libcis


void test_containers_sequences_array_indexing() {
  tests();
#if TEST_STD_VER >= 17
  static_assert(tests(), "");
#endif
  return;
}
