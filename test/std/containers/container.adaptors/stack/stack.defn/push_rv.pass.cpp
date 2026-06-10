// AST-transferred from libc++ by tools/transfer.py (slug=containers_container_adaptors_stack_stack_defn_push_rv).
// main -> test_containers_container_adaptors_stack_stack_defn_push_rv; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <stack>

// void push(value_type&& v);

#include <stack>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"

void test_containers_container_adaptors_stack_stack_defn_push_rv() {
  std::stack<MoveOnly> q;
  q.push(MoveOnly(1));
  assert(q.size() == 1);
  assert(q.top() == MoveOnly(1));
  q.push(MoveOnly(2));
  assert(q.size() == 2);
  assert(q.top() == MoveOnly(2));
  q.push(MoveOnly(3));
  assert(q.size() == 3);
  assert(q.top() == MoveOnly(3));

  return;
}
