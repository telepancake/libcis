// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_vector_bool_enabled_hash).
// main -> test_containers_sequences_vector_bool_enabled_hash; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <vector>

// Test that <vector> provides all of the arithmetic, enum, and pointer
// hash specializations.

#include <vector>

#include "poisoned_hash_helper.h"
#include "test_macros.h"
#include "min_allocator.h"

namespace libcis_ns_containers_sequences_vector_bool_enabled_hash { // libcis: isolate file-scope helpers
void test() {
  test_hash_enabled<std::vector<bool> >();
  test_hash_enabled<std::vector<bool, min_allocator<bool>>>();
}
} using namespace libcis_ns_containers_sequences_vector_bool_enabled_hash; // libcis


void test_containers_sequences_vector_bool_enabled_hash() {
  test_library_hash_specializations_available();
  test();

  return;
}
