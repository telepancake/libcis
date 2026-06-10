// AST-transferred from libc++ by tools/transfer.py (slug=containers_associative_multimap_multimap_cons_alloc).
// main -> test_containers_associative_multimap_multimap_cons_alloc; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <map>

// class multimap

// explicit multimap(const allocator_type& a);

#include <map>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"
#include "min_allocator.h"

void test_containers_associative_multimap_multimap_cons_alloc() {
  {
    typedef std::less<int> C;
    typedef test_allocator<std::pair<const int, double> > A;
    std::multimap<int, double, C, A> m(A(5));
    assert(m.empty());
    assert(m.begin() == m.end());
    assert(m.get_allocator() == A(5));
  }
#if TEST_STD_VER >= 11
  {
    typedef std::less<int> C;
    typedef min_allocator<std::pair<const int, double> > A;
    std::multimap<int, double, C, A> m(A{});
    assert(m.empty());
    assert(m.begin() == m.end());
    assert(m.get_allocator() == A());
  }
  {
    typedef std::less<int> C;
    typedef explicit_allocator<std::pair<const int, double> > A;
    std::multimap<int, double, C, A> m(A{});
    assert(m.empty());
    assert(m.begin() == m.end());
    assert(m.get_allocator() == A{});
  }
#endif

  return;
}
