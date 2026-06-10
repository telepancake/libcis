// AST-transferred from libc++ by tools/transfer.py (slug=containers_container_adaptors_queue_queue_defn_assign_copy_addressof).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <queue>

// queue& operator=(const queue& q);

// Validate whether the container can be copy-assigned with an ADL-hijacking operator&

#include <queue>

#include "test_macros.h"
#include "operator_hijacker.h"

namespace libcis_ns_containers_container_adaptors_queue_queue_defn_assign_copy_addressof { // libcis: isolate file-scope helpers
void test() {
  std::queue<operator_hijacker> qo;
  std::queue<operator_hijacker> q;
  q = qo;
}
} using namespace libcis_ns_containers_container_adaptors_queue_queue_defn_assign_copy_addressof; // libcis

