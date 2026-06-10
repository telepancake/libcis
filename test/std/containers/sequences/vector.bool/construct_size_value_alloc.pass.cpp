// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_vector_bool_construct_size_value_alloc).
// main -> test_containers_sequences_vector_bool_construct_size_value_alloc; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <vector>
// vector<bool>

// vector(size_type n, const value_type& x, const allocator_type& a);

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"

namespace libcis_ns_containers_sequences_vector_bool_construct_size_value_alloc { // libcis: isolate file-scope helpers
template <class C>
TEST_CONSTEXPR_CXX20 void
test(typename C::size_type n, const typename C::value_type& x, const typename C::allocator_type& a) {
  C c(n, x, a);
  LIBCPP_ASSERT(c.__invariants());
  assert(a == c.get_allocator());
  assert(c.size() == n);
  for (typename C::const_iterator i = c.cbegin(), e = c.cend(); i != e; ++i)
    assert(*i == x);
}

TEST_CONSTEXPR_CXX20 bool tests() {
  test<std::vector<bool> >(50, true, std::allocator<bool>());
#if TEST_STD_VER >= 11
  test<std::vector<bool, min_allocator<bool>> >(50, true, min_allocator<bool>());
#endif

  return true;
}
} using namespace libcis_ns_containers_sequences_vector_bool_construct_size_value_alloc; // libcis


void test_containers_sequences_vector_bool_construct_size_value_alloc() {
  tests();
#if TEST_STD_VER > 17
  static_assert(tests());
#endif
  return;
}
