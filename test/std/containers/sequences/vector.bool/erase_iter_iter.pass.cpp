// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_vector_bool_erase_iter_iter).
// main -> test_containers_sequences_vector_bool_erase_iter_iter; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <vector>
// vector<bool>

// iterator erase(const_iterator first, const_iterator last);

#include <vector>
#include <cassert>
#include <iterator>

#include "test_macros.h"
#include "min_allocator.h"

namespace libcis_ns_containers_sequences_vector_bool_erase_iter_iter { // libcis: isolate file-scope helpers
TEST_CONSTEXPR_CXX20 bool tests() {
  bool a1[] = {1, 0, 1};
  {
    std::vector<bool> l1(a1, a1 + 3);
    std::vector<bool>::iterator i = l1.erase(l1.cbegin(), l1.cbegin());
    assert(l1.size() == 3);
    assert(std::distance(l1.cbegin(), l1.cend()) == 3);
    assert(i == l1.begin());
  }
  {
    std::vector<bool> l1(a1, a1 + 3);
    std::vector<bool>::iterator i = l1.erase(l1.cbegin(), std::next(l1.cbegin()));
    assert(l1.size() == 2);
    assert(std::distance(l1.cbegin(), l1.cend()) == 2);
    assert(i == l1.begin());
    assert(l1 == std::vector<bool>(a1 + 1, a1 + 3));
  }
  {
    std::vector<bool> l1(a1, a1 + 3);
    std::vector<bool>::iterator i = l1.erase(l1.cbegin(), std::next(l1.cbegin(), 2));
    assert(l1.size() == 1);
    assert(std::distance(l1.cbegin(), l1.cend()) == 1);
    assert(i == l1.begin());
    assert(l1 == std::vector<bool>(a1 + 2, a1 + 3));
  }
  {
    std::vector<bool> l1(a1, a1 + 3);
    std::vector<bool>::iterator i = l1.erase(l1.cbegin(), std::next(l1.cbegin(), 3));
    assert(l1.size() == 0);
    assert(std::distance(l1.cbegin(), l1.cend()) == 0);
    assert(i == l1.begin());
  }
#if TEST_STD_VER >= 11
  {
    std::vector<bool, min_allocator<bool>> l1(a1, a1 + 3);
    std::vector<bool, min_allocator<bool>>::iterator i = l1.erase(l1.cbegin(), l1.cbegin());
    assert(l1.size() == 3);
    assert(std::distance(l1.cbegin(), l1.cend()) == 3);
    assert(i == l1.begin());
  }
  {
    std::vector<bool, min_allocator<bool>> l1(a1, a1 + 3);
    std::vector<bool, min_allocator<bool>>::iterator i = l1.erase(l1.cbegin(), std::next(l1.cbegin()));
    assert(l1.size() == 2);
    assert(std::distance(l1.cbegin(), l1.cend()) == 2);
    assert(i == l1.begin());
    assert((l1 == std::vector<bool, min_allocator<bool>>(a1 + 1, a1 + 3)));
  }
  {
    std::vector<bool, min_allocator<bool>> l1(a1, a1 + 3);
    std::vector<bool, min_allocator<bool>>::iterator i = l1.erase(l1.cbegin(), std::next(l1.cbegin(), 2));
    assert(l1.size() == 1);
    assert(std::distance(l1.cbegin(), l1.cend()) == 1);
    assert(i == l1.begin());
    assert((l1 == std::vector<bool, min_allocator<bool>>(a1 + 2, a1 + 3)));
  }
  {
    std::vector<bool, min_allocator<bool>> l1(a1, a1 + 3);
    std::vector<bool, min_allocator<bool>>::iterator i = l1.erase(l1.cbegin(), std::next(l1.cbegin(), 3));
    assert(l1.size() == 0);
    assert(std::distance(l1.cbegin(), l1.cend()) == 0);
    assert(i == l1.begin());
  }
#endif

  return true;
}
} using namespace libcis_ns_containers_sequences_vector_bool_erase_iter_iter; // libcis


void test_containers_sequences_vector_bool_erase_iter_iter() {
  tests();
#if TEST_STD_VER > 17
  static_assert(tests());
#endif
  return;
}
