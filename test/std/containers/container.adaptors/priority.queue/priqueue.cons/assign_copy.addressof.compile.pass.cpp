// transferred+adapted from libc++ by tools/transfer.py (slug=containers_container_adaptors_priority_queue_priqueue_cons_assign_copy_addressof_e4f105d4).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <queue>

// priority_queue& operator=(const priority_queue&) = default;

// Validate whether the container can be copy-assigned with an ADL-hijacking operator&

#include <queue>

#include "test_macros.h"
#include "operator_hijacker.h"

namespace libcis_ns_containers_container_adaptors_priority_queue_priqueue_cons_assign_copy_addressof_e4f105d4 { // libcis
void test() {
  std::priority_queue<operator_hijacker> pqo;
  std::priority_queue<operator_hijacker> pq;
  pq = pqo;
}
} // libcis_ns_containers_container_adaptors_priority_queue_priqueue_cons_assign_copy_addressof_e4f105d4 (libcis)

