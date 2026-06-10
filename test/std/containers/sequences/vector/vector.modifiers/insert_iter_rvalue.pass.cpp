// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_vector_vector_modifiers_insert_iter_rvalue).
// main -> test_containers_sequences_vector_vector_modifiers_insert_iter_rvalue; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03 && !stdlib=libc++

// <vector>

// iterator insert(const_iterator position, value_type&& x);

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"
#include "MoveOnly.h"
#include "min_allocator.h"
#include "asan_testing.h"

namespace libcis_ns_containers_sequences_vector_vector_modifiers_insert_iter_rvalue { // libcis: isolate file-scope helpers
TEST_CONSTEXPR_CXX20 bool tests() {
  {
    std::vector<MoveOnly> v(100);
    std::vector<MoveOnly>::iterator i = v.insert(v.cbegin() + 10, MoveOnly(3));
    assert(v.size() == 101);
    assert(is_contiguous_container_asan_correct(v));
    assert(i == v.begin() + 10);
    int j;
    for (j = 0; j < 10; ++j)
      assert(v[j] == MoveOnly());
    assert(v[j] == MoveOnly(3));
    for (++j; j < 101; ++j)
      assert(v[j] == MoveOnly());
  }
  {
    std::vector<MoveOnly, limited_allocator<MoveOnly, 300> > v(100);
    std::vector<MoveOnly, limited_allocator<MoveOnly, 300> >::iterator i = v.insert(v.cbegin() + 10, MoveOnly(3));
    assert(v.size() == 101);
    assert(is_contiguous_container_asan_correct(v));
    assert(i == v.begin() + 10);
    int j;
    for (j = 0; j < 10; ++j)
      assert(v[j] == MoveOnly());
    assert(v[j] == MoveOnly(3));
    for (++j; j < 101; ++j)
      assert(v[j] == MoveOnly());
  }
  {
    std::vector<MoveOnly, min_allocator<MoveOnly> > v(100);
    std::vector<MoveOnly, min_allocator<MoveOnly> >::iterator i = v.insert(v.cbegin() + 10, MoveOnly(3));
    assert(v.size() == 101);
    assert(is_contiguous_container_asan_correct(v));
    assert(i == v.begin() + 10);
    int j;
    for (j = 0; j < 10; ++j)
      assert(v[j] == MoveOnly());
    assert(v[j] == MoveOnly(3));
    for (++j; j < 101; ++j)
      assert(v[j] == MoveOnly());
  }
  {
    std::vector<MoveOnly, safe_allocator<MoveOnly> > v(100);
    std::vector<MoveOnly, safe_allocator<MoveOnly> >::iterator i = v.insert(v.cbegin() + 10, MoveOnly(3));
    assert(v.size() == 101);
    assert(is_contiguous_container_asan_correct(v));
    assert(i == v.begin() + 10);
    int j;
    for (j = 0; j < 10; ++j)
      assert(v[j] == MoveOnly());
    assert(v[j] == MoveOnly(3));
    for (++j; j < 101; ++j)
      assert(v[j] == MoveOnly());
  }

  return true;
}
} using namespace libcis_ns_containers_sequences_vector_vector_modifiers_insert_iter_rvalue; // libcis


void test_containers_sequences_vector_vector_modifiers_insert_iter_rvalue() {
  tests();
#if TEST_STD_VER > 17
  static_assert(tests());
#endif
  return;
}
