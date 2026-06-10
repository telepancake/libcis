// AST-transferred from libc++ by tools/transfer.py (slug=containers_container_adaptors_stack_stack_defn_pop).
// main -> test_containers_container_adaptors_stack_stack_defn_pop; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <stack>

// void pop();

#include <stack>
#include <cassert>

#include "test_macros.h"

void test_containers_container_adaptors_stack_stack_defn_pop() {
  std::stack<int> q;
  assert(q.size() == 0);
  q.push(1);
  q.push(2);
  q.push(3);
  assert(q.size() == 3);
  assert(q.top() == 3);
  q.pop();
  assert(q.size() == 2);
  assert(q.top() == 2);
  q.pop();
  assert(q.size() == 1);
  assert(q.top() == 1);
  q.pop();
  assert(q.size() == 0);

  return;
}
