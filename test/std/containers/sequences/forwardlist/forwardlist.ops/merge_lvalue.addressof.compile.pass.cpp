// transferred+adapted from libc++ by tools/transfer.py (slug=containers_sequences_forwardlist_forwardlist_ops_merge_lvalue_addressof_56d2fb9e).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <forward_list>

// template <class Compare> void merge(forward_list& x);

// Validate whether the operation properly guards against ADL-hijacking operator&

#include <forward_list>

#include "test_macros.h"
#include "operator_hijacker.h"

namespace libcis_ns_containers_sequences_forwardlist_forwardlist_ops_merge_lvalue_addressof_56d2fb9e { // libcis
void test() {
  std::forward_list<operator_hijacker> lo;
  std::forward_list<operator_hijacker> l;
  lo.merge(l);
}
} // libcis_ns_containers_sequences_forwardlist_forwardlist_ops_merge_lvalue_addressof_56d2fb9e (libcis)

