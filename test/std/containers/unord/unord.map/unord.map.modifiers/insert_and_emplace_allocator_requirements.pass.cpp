// transferred+adapted from libc++ by tools/transfer.py (slug=containers_unord_unord_map_unord_map_modifiers_insert_and_emplace_allocator_requirements_13503e34).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <unordered_map>

// class unordered_map

// insert(...);
// emplace(...);

// UNSUPPORTED: c++03


#include <unordered_map>

#include "test_macros.h"
#include "container_test_types.h"
#include "../../../map_allocator_requirement_test_templates.h"

namespace libcis_ns_containers_unord_unord_map_unord_map_modifiers_insert_and_emplace_allocator_requirements_13503e34 { // libcis
int main(int, char**)
{
  testMapInsert<TCT::unordered_map<> >();
  testMapInsertHint<TCT::unordered_map<> >();
  testMapEmplace<TCT::unordered_map<> >();
  testMapEmplaceHint<TCT::unordered_map<> >();

  return 0;

    return 0;
}
} // libcis_ns_containers_unord_unord_map_unord_map_modifiers_insert_and_emplace_allocator_requirements_13503e34 (libcis)

