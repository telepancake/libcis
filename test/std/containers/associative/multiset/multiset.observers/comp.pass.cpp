// AST-transferred from libc++ by tools/transfer.py (slug=containers_associative_multiset_multiset_observers_comp).
// main -> test_containers_associative_multiset_multiset_observers_comp; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <set>

// key_compare key_comp() const;
// value_compare value_comp() const;

#include <set>
#include <cassert>

void test_containers_associative_multiset_multiset_observers_comp() {
  typedef std::multiset<int> set_type;

  set_type s;
  set_type::iterator i1 = s.insert(1);
  set_type::iterator i2 = s.insert(2);

  const set_type& cs = s;

  assert(cs.key_comp()(*i1, *i2));
  assert(!cs.key_comp()(*i2, *i1));

  assert(cs.value_comp()(*i1, *i2));
  assert(!cs.value_comp()(*i2, *i1));

  return;
}
