// AST-transferred from libc++ by tools/transfer.py (slug=containers_container_adaptors_queue_queue_defn_back).
// main -> test_containers_container_adaptors_queue_queue_defn_back; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <queue>

// reference back();

#include <queue>
#include <cassert>

#include "test_macros.h"

void test_containers_container_adaptors_queue_queue_defn_back() {
  std::queue<int> q;
  assert(q.size() == 0);
  q.push(1);
  q.push(2);
  q.push(3);
  int& ir = q.back();
  assert(ir == 3);

  return;
}
