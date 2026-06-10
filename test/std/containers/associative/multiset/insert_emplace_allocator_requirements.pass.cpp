// AST-transferred from libc++ by tools/transfer.py (slug=containers_associative_multiset_insert_emplace_allocator_requirements).
// main -> test_containers_associative_multiset_insert_emplace_allocator_requirements; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <set>

// class multiset

// insert(...)

// UNSUPPORTED: c++03

#include <set>

#include "test_macros.h"
#include "container_test_types.h"
#include "../../set_allocator_requirement_test_templates.h"

void test_containers_associative_multiset_insert_emplace_allocator_requirements() {
  testMultisetInsert<TCT::multiset<> >();
  testMultisetEmplace<TCT::multiset<> >();

  return;
}
