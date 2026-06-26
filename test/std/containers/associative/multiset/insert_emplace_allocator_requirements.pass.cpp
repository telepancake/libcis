// transferred+adapted from libc++ by tools/transfer.py (slug=containers_associative_multiset_insert_emplace_allocator_requirements_4ac82170).
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

namespace libcis_ns_containers_associative_multiset_insert_emplace_allocator_requirements_4ac82170 { // libcis
int main(int, char**)
{
  testMultisetInsert<TCT::multiset<> >();
  testMultisetEmplace<TCT::multiset<> >();

  return 0;

    return 0;
}
} // libcis_ns_containers_associative_multiset_insert_emplace_allocator_requirements_4ac82170 (libcis)

