// AST-transferred from libc++ by tools/transfer.py (slug=containers_container_adaptors_stack_stack_defn_push).
// main -> test_containers_container_adaptors_stack_stack_defn_push; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <stack>

// void push(const value_type& v);

#include <stack>
#include <cassert>

#include "test_macros.h"

void test_containers_container_adaptors_stack_stack_defn_push() {
  std::stack<int> q;
  q.push(1);
  assert(q.size() == 1);
  assert(q.top() == 1);
  q.push(2);
  assert(q.size() == 2);
  assert(q.top() == 2);
  q.push(3);
  assert(q.size() == 3);
  assert(q.top() == 3);

  return;
}
