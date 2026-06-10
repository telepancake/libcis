// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_forwardlist_forwardlist_cons_assign_copy_addressof).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <forward_list>

// forward_list& operator=(const forward_list& x);

// Validate whether the container can be copy-assigned with an ADL-hijacking operator&

#include <forward_list>

#include "test_macros.h"
#include "operator_hijacker.h"

namespace libcis_ns_containers_sequences_forwardlist_forwardlist_cons_assign_copy_addressof { // libcis: isolate file-scope helpers
void test() {
  std::forward_list<operator_hijacker> lo;
  std::forward_list<operator_hijacker> l;
  l = lo;
}
} using namespace libcis_ns_containers_sequences_forwardlist_forwardlist_cons_assign_copy_addressof; // libcis

