// transferred+adapted from libc++ by tools/transfer.py (slug=containers_unord_unord_set_insert_and_emplace_allocator_requirements_7729c778).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <unordered_set>

// class unordered_set

// insert(...)
// emplace(...)

// UNSUPPORTED: c++03

#include <unordered_set>

#include "test_macros.h"
#include "container_test_types.h"
#include "../../set_allocator_requirement_test_templates.h"


namespace libcis_ns_containers_unord_unord_set_insert_and_emplace_allocator_requirements_7729c778 { // libcis
int main(int, char**)
{
  testSetInsert<TCT::unordered_set<> >();
  testSetEmplace<TCT::unordered_set<> >();

  return 0;

    return 0;
}
} // libcis_ns_containers_unord_unord_set_insert_and_emplace_allocator_requirements_7729c778 (libcis)

