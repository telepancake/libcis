// AST-transferred from libc++ by tools/transfer.py (slug=containers_container_adaptors_queue_queue_defn_swap).
// main -> test_containers_container_adaptors_queue_queue_defn_swap; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <queue>

// void swap(queue& q);

#include <queue>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_containers_container_adaptors_queue_queue_defn_swap { // libcis: isolate file-scope helpers
template <class C>
C make(int n) {
  C c;
  for (int i = 0; i < n; ++i)
    c.push(i);
  return c;
}
} using namespace libcis_ns_containers_container_adaptors_queue_queue_defn_swap; // libcis


void test_containers_container_adaptors_queue_queue_defn_swap() {
  std::queue<int> q1      = make<std::queue<int> >(5);
  std::queue<int> q2      = make<std::queue<int> >(10);
  std::queue<int> q1_save = q1;
  std::queue<int> q2_save = q2;
  q1.swap(q2);
  assert(q1 == q2_save);
  assert(q2 == q1_save);

  return;
}
