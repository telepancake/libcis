// transferred+adapted from libc++ by tools/transfer.py (slug=containers_associative_multiset_multiset_cons_copy_assign_addressof_9fd30ef7).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <set>

// class multiset

// multiset& operator=(const multiset& s);

// Validate whether the container can be copy-assigned with an ADL-hijacking operator&

#include <set>

#include "test_macros.h"
#include "operator_hijacker.h"

namespace libcis_ns_containers_associative_multiset_multiset_cons_copy_assign_addressof_9fd30ef7 { // libcis
void test() {
  std::multiset<operator_hijacker> so;
  std::multiset<operator_hijacker> s;
  s = so;
}
} // libcis_ns_containers_associative_multiset_multiset_cons_copy_assign_addressof_9fd30ef7 (libcis)

