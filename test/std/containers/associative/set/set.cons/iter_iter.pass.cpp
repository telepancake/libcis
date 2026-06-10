// AST-transferred from libc++ by tools/transfer.py (slug=containers_associative_set_set_cons_iter_iter).
// main -> test_containers_associative_set_set_cons_iter_iter; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <set>

// class set

// template <class InputIterator>
//     set(InputIterator first, InputIterator last);

#include <set>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "min_allocator.h"

void test_containers_associative_set_set_cons_iter_iter() {
  {
    typedef int V;
    V ar[] = {1, 1, 1, 2, 2, 2, 3, 3, 3};
    std::set<V> m(
        cpp17_input_iterator<const int*>(ar), cpp17_input_iterator<const int*>(ar + sizeof(ar) / sizeof(ar[0])));
    assert(m.size() == 3);
    assert(std::distance(m.begin(), m.end()) == 3);
    assert(*m.begin() == 1);
    assert(*std::next(m.begin()) == 2);
    assert(*std::next(m.begin(), 2) == 3);
  }
#if TEST_STD_VER >= 11
  {
    typedef int V;
    V ar[] = {1, 1, 1, 2, 2, 2, 3, 3, 3};
    std::set<V, std::less<int>, min_allocator<int>> m(
        cpp17_input_iterator<const int*>(ar), cpp17_input_iterator<const int*>(ar + sizeof(ar) / sizeof(ar[0])));
    assert(m.size() == 3);
    assert(std::distance(m.begin(), m.end()) == 3);
    assert(*m.begin() == 1);
    assert(*std::next(m.begin()) == 2);
    assert(*std::next(m.begin(), 2) == 3);
  }
#endif

  return;
}
