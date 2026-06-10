// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_forwardlist_forwardlist_ops_merge_lvalue_pred_addressof).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <forward_list>

// template <class Compare> void merge(forward_list& x, Compare comp);

// Validate whether the operation properly guards against ADL-hijacking operator&

#include <forward_list>

#include "test_macros.h"
#include "operator_hijacker.h"

namespace libcis_ns_containers_sequences_forwardlist_forwardlist_ops_merge_lvalue_pred_addressof { // libcis: isolate file-scope helpers
void test() {
  std::forward_list<operator_hijacker> lo;
  std::forward_list<operator_hijacker> l;
  lo.merge(l, std::less<operator_hijacker>());
}
} using namespace libcis_ns_containers_sequences_forwardlist_forwardlist_ops_merge_lvalue_pred_addressof; // libcis

