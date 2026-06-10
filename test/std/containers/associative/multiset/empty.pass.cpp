// AST-transferred from libc++ by tools/transfer.py (slug=containers_associative_multiset_empty).
// main -> test_containers_associative_multiset_empty; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <set>

// class multiset

// bool empty() const;

#include <set>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"

void test_containers_associative_multiset_empty() {
  {
    typedef std::multiset<int> M;
    M m;
    assert(m.empty());
    m.insert(M::value_type(1));
    assert(!m.empty());
    m.clear();
    assert(m.empty());
  }
#if TEST_STD_VER >= 11
  {
    typedef std::multiset<int, std::less<int>, min_allocator<int>> M;
    M m;
    assert(m.empty());
    m.insert(M::value_type(1));
    assert(!m.empty());
    m.clear();
    assert(m.empty());
  }
#endif

  return;
}
