// transferred+adapted from libc++ by tools/transfer.py (slug=containers_sequences_list_list_ops_merge_comp_addressof_87c95f38).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <list>

// template <class Compare> void merge(list& x, Compare comp);
// If (addressof(x) == this) does nothing; otherwise ...

// Validate whether the operation properly guards against ADL-hijacking operator&

#include <list>

#include "test_macros.h"
#include "operator_hijacker.h"

namespace libcis_ns_containers_sequences_list_list_ops_merge_comp_addressof_87c95f38 { // libcis
void test() {
  std::list<operator_hijacker> l;
  l.merge(l, std::less<operator_hijacker>());
}
} // libcis_ns_containers_sequences_list_list_ops_merge_comp_addressof_87c95f38 (libcis)

