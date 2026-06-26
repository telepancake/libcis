// transferred+adapted from libc++ by tools/transfer.py (slug=containers_sequences_list_list_cons_assign_move_addressof_05bee1a3).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <list>

// list& operator=(list&& c);

// Validate whether the operation properly guards against ADL-hijacking operator&

#include <list>

#include "test_macros.h"
#include "operator_hijacker.h"

namespace libcis_ns_containers_sequences_list_list_cons_assign_move_addressof_05bee1a3 { // libcis
void test() {
  std::list<operator_hijacker> lo;
  std::list<operator_hijacker> l;
  l = std::move(lo);
}
} // libcis_ns_containers_sequences_list_list_cons_assign_move_addressof_05bee1a3 (libcis)

