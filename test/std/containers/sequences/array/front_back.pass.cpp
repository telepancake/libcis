// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_array_front_back).
// main -> test_containers_sequences_array_front_back; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <array>

// reference front();  // constexpr in C++17
// reference back();   // constexpr in C++17

#include <array>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_containers_sequences_array_front_back { // libcis: isolate file-scope helpers
TEST_CONSTEXPR_CXX17 bool tests() {
  {
    typedef double T;
    typedef std::array<T, 3> C;
    C c = {1, 2, 3.5};

    C::reference r1 = c.front();
    assert(r1 == 1);
    r1 = 5.5;
    assert(c[0] == 5.5);

    C::reference r2 = c.back();
    assert(r2 == 3.5);
    r2 = 7.5;
    assert(c[2] == 7.5);
  }
  {
    typedef double T;
    typedef std::array<T, 0> C;
    C c = {};
    ASSERT_SAME_TYPE(decltype(c.back()), C::reference);
    LIBCPP_ASSERT_NOEXCEPT(c.back());
    ASSERT_SAME_TYPE(decltype(c.front()), C::reference);
    LIBCPP_ASSERT_NOEXCEPT(c.front());
    if (c.size() > (0)) { // always false
      TEST_IGNORE_NODISCARD c.front();
      TEST_IGNORE_NODISCARD c.back();
    }
  }
  {
    typedef double T;
    typedef std::array<const T, 0> C;
    C c = {};
    ASSERT_SAME_TYPE(decltype(c.back()), C::reference);
    LIBCPP_ASSERT_NOEXCEPT(c.back());
    ASSERT_SAME_TYPE(decltype(c.front()), C::reference);
    LIBCPP_ASSERT_NOEXCEPT(c.front());
    if (c.size() > (0)) {
      TEST_IGNORE_NODISCARD c.front();
      TEST_IGNORE_NODISCARD c.back();
    }
  }

  return true;
}
} using namespace libcis_ns_containers_sequences_array_front_back; // libcis


void test_containers_sequences_array_front_back() {
  tests();
#if TEST_STD_VER >= 17
  static_assert(tests(), "");
#endif
  return;
}
