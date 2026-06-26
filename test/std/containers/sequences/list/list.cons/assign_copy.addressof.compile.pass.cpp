// transferred+adapted from libc++ by tools/transfer.py (slug=containers_sequences_list_list_cons_assign_copy_addressof_988c320b).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <list>

// list& operator=(const list& c);

// Validate whether the container can be copy-assigned with an ADL-hijacking operator&

#include <list>

#include "test_macros.h"
#include "operator_hijacker.h"

namespace libcis_ns_containers_sequences_list_list_cons_assign_copy_addressof_988c320b { // libcis
void test() {
  std::list<operator_hijacker> lo;
  std::list<operator_hijacker> l;
  l = lo;
}
} // libcis_ns_containers_sequences_list_list_cons_assign_copy_addressof_988c320b (libcis)

