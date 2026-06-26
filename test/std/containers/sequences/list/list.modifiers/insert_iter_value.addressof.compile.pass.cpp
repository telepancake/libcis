// transferred+adapted from libc++ by tools/transfer.py (slug=containers_sequences_list_list_modifiers_insert_iter_value_addressof_b2449e07).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <list>

// iterator insert(const_iterator position, const value_type& x);

// Validate whether the operation properly guards against ADL-hijacking operator&

#include <list>

#include "test_macros.h"
#include "operator_hijacker.h"

namespace libcis_ns_containers_sequences_list_list_modifiers_insert_iter_value_addressof_b2449e07 { // libcis
void test(std::list<operator_hijacker>& l) { l.insert(l.begin(), l.front()); }
} // libcis_ns_containers_sequences_list_list_modifiers_insert_iter_value_addressof_b2449e07 (libcis)

