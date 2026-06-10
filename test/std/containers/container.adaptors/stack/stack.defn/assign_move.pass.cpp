// AST-transferred from libc++ by tools/transfer.py (slug=containers_container_adaptors_stack_stack_defn_assign_move).
// main -> test_containers_container_adaptors_stack_stack_defn_assign_move; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <stack>

// stack& operator=(stack&& q);

#include <stack>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"

namespace libcis_ns_containers_container_adaptors_stack_stack_defn_assign_move { // libcis: isolate file-scope helpers
template <class C>
C make(int n) {
  C c;
  for (int i = 0; i < n; ++i)
    c.push_back(MoveOnly(i));
  return c;
}
} using namespace libcis_ns_containers_container_adaptors_stack_stack_defn_assign_move; // libcis


void test_containers_container_adaptors_stack_stack_defn_assign_move() {
  std::stack<MoveOnly> q(make<std::deque<MoveOnly> >(5));
  std::stack<MoveOnly> q2;
  q2 = std::move(q);
  assert(q2.size() == 5);
  assert(q.empty());

  return;
}
