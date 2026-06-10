// AST-transferred from libc++ by tools/transfer.py (slug=containers_container_adaptors_stack_stack_defn_assign_copy).
// main -> test_containers_container_adaptors_stack_stack_defn_assign_copy; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <stack>

// stack& operator=(const stack& q);

#include <stack>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_containers_container_adaptors_stack_stack_defn_assign_copy { // libcis: isolate file-scope helpers
template <class C>
C make(int n) {
  C c;
  for (int i = 0; i < n; ++i)
    c.push_back(i);
  return c;
}
} using namespace libcis_ns_containers_container_adaptors_stack_stack_defn_assign_copy; // libcis


void test_containers_container_adaptors_stack_stack_defn_assign_copy() {
  std::stack<int> q(make<std::deque<int> >(5));
  std::stack<int> q2;
  q2 = q;
  assert(q2 == q);

  return;
}
