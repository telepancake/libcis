// AST-transferred from libc++ by tools/transfer.py (slug=containers_container_adaptors_stack_stack_defn_empty).
// main -> test_containers_container_adaptors_stack_stack_defn_empty; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <stack>

// bool empty() const;

#include <stack>
#include <cassert>

#include "test_macros.h"

void test_containers_container_adaptors_stack_stack_defn_empty() {
  std::stack<int> q;
  assert(q.empty());
  q.push(1);
  assert(!q.empty());
  q.pop();
  assert(q.empty());

  return;
}
