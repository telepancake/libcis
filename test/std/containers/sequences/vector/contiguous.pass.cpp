// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_vector_contiguous).
// main -> test_containers_sequences_vector_contiguous; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <vector>

// An vector is a contiguous container

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"
#include "min_allocator.h"

namespace libcis_ns_containers_sequences_vector_contiguous { // libcis: isolate file-scope helpers
template <class C>
TEST_CONSTEXPR_CXX20 void test_contiguous(const C& c) {
  for (std::size_t i = 0; i < c.size(); ++i)
    assert(*(c.begin() + static_cast<typename C::difference_type>(i)) == *(std::addressof(*c.begin()) + i));
}

TEST_CONSTEXPR_CXX20 bool tests() {
  {
    typedef int T;
    typedef std::vector<T> C;
    test_contiguous(C());
    test_contiguous(C(3, 5));
  }

  {
    typedef double T;
    typedef test_allocator<T> A;
    typedef std::vector<T, A> C;
    test_contiguous(C(A(3)));
    test_contiguous(C(7, 9.0, A(5)));
  }
#if TEST_STD_VER >= 11
  {
    typedef double T;
    typedef min_allocator<T> A;
    typedef std::vector<T, A> C;
    test_contiguous(C(A{}));
    test_contiguous(C(9, 11.0, A{}));
  }
  {
    typedef double T;
    typedef safe_allocator<T> A;
    typedef std::vector<T, A> C;
    test_contiguous(C(A{}));
    test_contiguous(C(9, 11.0, A{}));
  }
#endif

  return true;
}
} using namespace libcis_ns_containers_sequences_vector_contiguous; // libcis


void test_containers_sequences_vector_contiguous() {
  tests();
#if TEST_STD_VER > 17
  static_assert(tests());
#endif
  return;
}
