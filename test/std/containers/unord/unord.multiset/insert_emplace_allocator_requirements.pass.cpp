// transferred+adapted from libc++ by tools/transfer.py (slug=containers_unord_unord_multiset_insert_emplace_allocator_requirements_6c84875e).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <unordered_set>

// class unordered_multiset

// insert(...)

// UNSUPPORTED: c++03

#include <unordered_set>
#include "test_macros.h"
#include "container_test_types.h"
#include "../../set_allocator_requirement_test_templates.h"

namespace libcis_ns_containers_unord_unord_multiset_insert_emplace_allocator_requirements_6c84875e { // libcis
int main(int, char**)
{
  testMultisetInsert<TCT::unordered_multiset<> >();
  testMultisetEmplace<TCT::unordered_multiset<> >();

  return 0;

    return 0;
}
} // libcis_ns_containers_unord_unord_multiset_insert_emplace_allocator_requirements_6c84875e (libcis)

