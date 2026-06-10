// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_vector_vector_cons_default_noexcept).
// main -> test_containers_sequences_vector_vector_cons_default_noexcept; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03

// <vector>

// vector()
//        noexcept(is_nothrow_default_constructible<allocator_type>::value);

// This *was* a conforming extension, but it was adopted in N4258.

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"
#include "test_allocator.h"

namespace libcis_ns_containers_sequences_vector_vector_cons_default_noexcept { // libcis: isolate file-scope helpers
template <class T>
struct some_alloc {
  typedef T value_type;
  some_alloc(const some_alloc&);
  void allocate(std::size_t);
};

TEST_CONSTEXPR_CXX20 bool tests() {
  {
    typedef std::vector<MoveOnly> C;
    static_assert(std::is_nothrow_default_constructible<C>::value, "");
  }
  {
    typedef std::vector<MoveOnly, test_allocator<MoveOnly>> C;
    static_assert(std::is_nothrow_default_constructible<C>::value, "");
  }
  {
    typedef std::vector<MoveOnly, other_allocator<MoveOnly>> C;
    static_assert(!std::is_nothrow_default_constructible<C>::value, "");
  }
  {
    typedef std::vector<MoveOnly, some_alloc<MoveOnly>> C;
    static_assert(!std::is_nothrow_default_constructible<C>::value, "");
  }

  return true;
}
} using namespace libcis_ns_containers_sequences_vector_vector_cons_default_noexcept; // libcis


void test_containers_sequences_vector_vector_cons_default_noexcept() {
  tests();
#if TEST_STD_VER > 17
  static_assert(tests());
#endif
  return;
}
