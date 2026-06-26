// transferred+adapted from libc++ by tools/transfer.py (slug=containers_associative_set_set_cons_copy_assign_addressof_09cff28a).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <set>

// class set

// set& operator=(const set& s);

// Validate whether the container can be copy-assigned with an ADL-hijacking operator&

#include <set>

#include "test_macros.h"
#include "operator_hijacker.h"

namespace libcis_ns_containers_associative_set_set_cons_copy_assign_addressof_09cff28a { // libcis
void test() {
  std::set<operator_hijacker> so;
  std::set<operator_hijacker> s;
  s = so;
}
} // libcis_ns_containers_associative_set_set_cons_copy_assign_addressof_09cff28a (libcis)

