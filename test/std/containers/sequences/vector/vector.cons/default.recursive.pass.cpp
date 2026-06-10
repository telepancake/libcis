// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_vector_vector_cons_default_recursive).
// main -> test_containers_sequences_vector_vector_cons_default_recursive; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <vector>
// class vector
// vector();

#include <vector>

#include "test_macros.h"

namespace libcis_ns_containers_sequences_vector_vector_cons_default_recursive { // libcis: isolate file-scope helpers
struct X {
  std::vector<X> q;
};
} using namespace libcis_ns_containers_sequences_vector_vector_cons_default_recursive; // libcis


void test_containers_sequences_vector_vector_cons_default_recursive() { return; }
