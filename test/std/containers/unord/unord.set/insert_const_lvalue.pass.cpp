// AST-transferred from libc++ by tools/transfer.py (slug=containers_unord_unord_set_insert_const_lvalue).
// main -> test_containers_unord_unord_set_insert_const_lvalue; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <unordered_set>

// template <class Value, class Hash = hash<Value>, class Pred = equal_to<Value>,
//           class Alloc = allocator<Value>>
// class unordered_set

// pair<iterator, bool> insert(const value_type& x);

#include <unordered_set>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"

namespace libcis_ns_containers_unord_unord_set_insert_const_lvalue { // libcis: isolate file-scope helpers
template <class Container>
void do_insert_const_lvalue_test() {
  typedef Container C;
  typedef std::pair<typename C::iterator, bool> R;
  typedef typename C::value_type VT;
  C c;
  const VT v1(3.5);
  R r = c.insert(v1);
  assert(c.size() == 1);
  assert(*r.first == 3.5);
  assert(r.second);

  r = c.insert(v1);
  assert(c.size() == 1);
  assert(*r.first == 3.5);
  assert(!r.second);

  const VT v2(4.5);
  r = c.insert(v2);
  assert(c.size() == 2);
  assert(*r.first == 4.5);
  assert(r.second);

  const VT v3(5.5);
  r = c.insert(v3);
  assert(c.size() == 3);
  assert(*r.first == 5.5);
  assert(r.second);
}
} using namespace libcis_ns_containers_unord_unord_set_insert_const_lvalue; // libcis


void test_containers_unord_unord_set_insert_const_lvalue() {
  do_insert_const_lvalue_test<std::unordered_set<double> >();
#if TEST_STD_VER >= 11
  {
    typedef std::unordered_set<double, std::hash<double>, std::equal_to<double>, min_allocator<double>> C;
    do_insert_const_lvalue_test<C>();
  }
#endif

  return;
}
