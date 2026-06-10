// AST-transferred from libc++ by tools/transfer.py (slug=containers_associative_set_set_cons_alloc).
// main -> test_containers_associative_set_set_cons_alloc; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <set>

// class set

// set(const allocator_type& a);

#include <set>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"

void test_containers_associative_set_set_cons_alloc() {
  typedef std::less<int> C;
  typedef test_allocator<int> A;
  std::set<int, C, A> m(A(5));
  assert(m.empty());
  assert(m.begin() == m.end());
  assert(m.get_allocator() == A(5));

  return;
}
