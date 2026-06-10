// AST-transferred from libc++ by tools/transfer.py (slug=containers_associative_multimap_multimap_cons_default).
// main -> test_containers_associative_multimap_multimap_cons_default; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <map>

// class multimap

// multimap();

#include <map>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"

void test_containers_associative_multimap_multimap_cons_default() {
  {
    std::multimap<int, double> m;
    assert(m.empty());
    assert(m.begin() == m.end());
  }
#if TEST_STD_VER >= 11
  {
    std::multimap<int, double, std::less<int>, min_allocator<std::pair<const int, double>>> m;
    assert(m.empty());
    assert(m.begin() == m.end());
  }
  {
    typedef explicit_allocator<std::pair<const int, double>> A;
    {
      std::multimap<int, double, std::less<int>, A> m;
      assert(m.empty());
      assert(m.begin() == m.end());
    }
    {
      A a;
      std::multimap<int, double, std::less<int>, A> m(a);
      assert(m.empty());
      assert(m.begin() == m.end());
    }
  }
  {
    std::multimap<int, double> m = {};
    assert(m.empty());
    assert(m.begin() == m.end());
  }
#endif

  return;
}
