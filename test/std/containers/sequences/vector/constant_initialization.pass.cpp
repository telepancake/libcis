// transferred+adapted from libc++ by tools/transfer.py (slug=containers_sequences_vector_constant_initialization_90a772ff).
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

namespace libcis_ns_containers_sequences_vector_constant_initialization_90a772ff { // libcis
std::vector<int> ca_allocs;

int main(int, char**) {
  ca_allocs.push_back(0);
  for ([[maybe_unused]] const auto& a : ca_allocs)
    ;

  return 0;

    return 0;
}
} // libcis_ns_containers_sequences_vector_constant_initialization_90a772ff (libcis)

