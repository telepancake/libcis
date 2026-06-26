// transferred+adapted from libc++ by tools/transfer.py (slug=containers_associative_map_map_modifiers_insert_and_emplace_allocator_requirements_4b8349cc).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <map>

// class map

// insert(...);
// emplace(...);
// emplace_hint(...);

// UNSUPPORTED: c++03

#include <map>

#include "test_macros.h"
#include "container_test_types.h"
#include "../../../map_allocator_requirement_test_templates.h"

namespace libcis_ns_containers_associative_map_map_modifiers_insert_and_emplace_allocator_requirements_4b8349cc { // libcis
int main(int, char**)
{
  testMapInsert<TCT::map<> >();
  testMapInsertHint<TCT::map<> >();
  testMapEmplace<TCT::map<> >();
  testMapEmplaceHint<TCT::map<> >();

  return 0;

    return 0;
}
} // libcis_ns_containers_associative_map_map_modifiers_insert_and_emplace_allocator_requirements_4b8349cc (libcis)

