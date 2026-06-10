// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_list_list_modifiers_emplace_addressof).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <list>

// template <class... Args> void emplace(const_iterator p, Args&&... args);

// Validate whether the operation properly guards against ADL-hijacking operator&

#include <list>

#include "test_macros.h"
#include "operator_hijacker.h"

namespace libcis_ns_containers_sequences_list_list_modifiers_emplace_addressof { // libcis: isolate file-scope helpers
void test(std::list<operator_hijacker>& l) { l.emplace(l.begin(), l.front()); }
} using namespace libcis_ns_containers_sequences_list_list_modifiers_emplace_addressof; // libcis

