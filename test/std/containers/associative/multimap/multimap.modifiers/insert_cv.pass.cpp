// AST-transferred from libc++ by tools/transfer.py (slug=containers_associative_multimap_multimap_modifiers_insert_cv).
// main -> test_containers_associative_multimap_multimap_modifiers_insert_cv; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <map>

// class multimap

// iterator insert(const value_type& v);

#include <map>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"

namespace libcis_ns_containers_associative_multimap_multimap_modifiers_insert_cv { // libcis: isolate file-scope helpers
template <class Container>
void do_insert_test() {
  typedef Container M;
  typedef typename M::iterator R;
  typedef typename M::value_type VT;
  M m;
  const VT v1(2, 2.5);
  R r = m.insert(v1);
  assert(r == m.begin());
  assert(m.size() == 1);
  assert(r->first == 2);
  assert(r->second == 2.5);

  const VT v2(1, 1.5);
  r = m.insert(v2);
  assert(r == m.begin());
  assert(m.size() == 2);
  assert(r->first == 1);
  assert(r->second == 1.5);

  const VT v3(3, 3.5);
  r = m.insert(v3);
  assert(r == std::prev(m.end()));
  assert(m.size() == 3);
  assert(r->first == 3);
  assert(r->second == 3.5);

  const VT v4(3, 3.5);
  r = m.insert(v4);
  assert(r == std::prev(m.end()));
  assert(m.size() == 4);
  assert(r->first == 3);
  assert(r->second == 3.5);
}
} using namespace libcis_ns_containers_associative_multimap_multimap_modifiers_insert_cv; // libcis


void test_containers_associative_multimap_multimap_modifiers_insert_cv() {
  {
    typedef std::multimap<int, double> Container;
    do_insert_test<Container>();
  }
#if TEST_STD_VER >= 11
  {
    typedef std::multimap<int, double, std::less<int>, min_allocator<std::pair<const int, double>>> Container;
    do_insert_test<Container>();
  }
#endif

  return;
}
