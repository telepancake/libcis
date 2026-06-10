// AST-transferred from libc++ by tools/transfer.py (slug=containers_associative_multimap_multimap_modifiers_insert_allocator_requirements).
// main -> test_containers_associative_multimap_multimap_modifiers_insert_allocator_requirements; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <map>

// class multimap

// insert(...)

// UNSUPPORTED: c++03

#include <map>

#include "test_macros.h"
#include "container_test_types.h"
#include "../../../map_allocator_requirement_test_templates.h"

void test_containers_associative_multimap_multimap_modifiers_insert_allocator_requirements() {
  testMultimapInsert<TCT::multimap<> >();
  testMultimapInsertHint<TCT::multimap<> >();

  return;
}
