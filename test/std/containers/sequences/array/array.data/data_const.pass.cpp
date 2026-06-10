// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_array_array_data_data_const).
// main -> test_containers_sequences_array_array_data_data_const; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <array>

// const T* data() const;

#include <array>
#include <cassert>
#include <cstddef> // for std::max_align_t
#include <cstdint>

#include "test_macros.h"

namespace libcis_ns_containers_sequences_array_array_data_data_const { // libcis: isolate file-scope helpers
struct NoDefault {
  TEST_CONSTEXPR NoDefault(int) {}
};

#if TEST_STD_VER < 11
struct natural_alignment {
  long t1;
  long long t2;
  double t3;
  long double t4;
};
#endif

TEST_CONSTEXPR_CXX17 bool tests() {
  {
    typedef double T;
    typedef std::array<T, 3> C;
    const C c = {1, 2, 3.5};
    ASSERT_NOEXCEPT(c.data());
    const T* p = c.data();
    assert(p[0] == 1);
    assert(p[1] == 2);
    assert(p[2] == 3.5);
  }
  {
    typedef double T;
    typedef std::array<T, 0> C;
    const C c = {};
    ASSERT_NOEXCEPT(c.data());
    const T* p = c.data();
    (void)p;
  }
  {
    typedef NoDefault T;
    typedef std::array<T, 0> C;
    const C c = {};
    ASSERT_NOEXCEPT(c.data());
    const T* p = c.data();
    (void)p;
  }
  {
    std::array<int, 5> const c = {0, 1, 2, 3, 4};
    assert(c.data() == &c[0]);
    assert(*c.data() == c[0]);
  }

  return true;
}
} using namespace libcis_ns_containers_sequences_array_array_data_data_const; // libcis


void test_containers_sequences_array_array_data_data_const() {
  tests();
#if TEST_STD_VER >= 17
  static_assert(tests(), "");
#endif

  // Test the alignment of data()
  {
#if TEST_STD_VER < 11
    typedef natural_alignment T;
#else
    typedef std::max_align_t T;
#endif
    typedef std::array<T, 0> C;
    const C c           = {};
    const T* p          = c.data();
    std::uintptr_t pint = reinterpret_cast<std::uintptr_t>(p);
    assert(pint % TEST_ALIGNOF(T) == 0);
  }

  return;
}
