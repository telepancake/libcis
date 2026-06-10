// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_vector_constant_initialization).
// main -> test_containers_sequences_vector_constant_initialization; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

#include <algorithm>
#include <vector>

namespace libcis_ns_containers_sequences_vector_constant_initialization { // libcis: isolate file-scope helpers
std::vector<int> ca_allocs;
} using namespace libcis_ns_containers_sequences_vector_constant_initialization; // libcis


void test_containers_sequences_vector_constant_initialization() {
  ca_allocs.push_back(0);
  for ([[maybe_unused]] const auto& a : ca_allocs)
    ;

  return;
}
