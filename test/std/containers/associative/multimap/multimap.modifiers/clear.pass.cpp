// AST-transferred from libc++ by tools/transfer.py (slug=containers_associative_multimap_multimap_modifiers_clear).
// main -> test_containers_associative_multimap_multimap_modifiers_clear; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <map>

// class multimap

// void clear() noexcept;

#include <map>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"

void test_containers_associative_multimap_multimap_modifiers_clear() {
  {
    typedef std::multimap<int, double> M;
    typedef std::pair<int, double> P;
    P ar[] = {
        P(1, 1.5),
        P(2, 2.5),
        P(3, 3.5),
        P(4, 4.5),
        P(5, 5.5),
        P(6, 6.5),
        P(7, 7.5),
        P(8, 8.5),
    };
    M m(ar, ar + sizeof(ar) / sizeof(ar[0]));
    assert(m.size() == 8);
    ASSERT_NOEXCEPT(m.clear());
    m.clear();
    assert(m.size() == 0);
  }
#if TEST_STD_VER >= 11
  {
    typedef std::multimap<int, double, std::less<int>, min_allocator<std::pair<const int, double>>> M;
    typedef std::pair<int, double> P;
    P ar[] = {
        P(1, 1.5),
        P(2, 2.5),
        P(3, 3.5),
        P(4, 4.5),
        P(5, 5.5),
        P(6, 6.5),
        P(7, 7.5),
        P(8, 8.5),
    };
    M m(ar, ar + sizeof(ar) / sizeof(ar[0]));
    assert(m.size() == 8);
    ASSERT_NOEXCEPT(m.clear());
    m.clear();
    assert(m.size() == 0);
  }
#endif

  return;
}
