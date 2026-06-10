// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_deque_deque_cons_move_assign_addressof).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <deque>

// deque& operator=(deque&& c);

// Validate whether the container can be copy-assigned with an ADL-hijacking operator&

#include <deque>

#include "test_macros.h"
#include "operator_hijacker.h"

namespace libcis_ns_containers_sequences_deque_deque_cons_move_assign_addressof { // libcis: isolate file-scope helpers
void test() {
  std::deque<operator_hijacker> dqo;
  std::deque<operator_hijacker> dq;
  dq = dqo;
}
} using namespace libcis_ns_containers_sequences_deque_deque_cons_move_assign_addressof; // libcis

