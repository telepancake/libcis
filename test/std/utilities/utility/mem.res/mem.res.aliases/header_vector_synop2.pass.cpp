// AST-transferred from libc++ by tools/transfer.py (slug=utilities_utility_mem_res_mem_res_aliases_header_vector_synop2).
// main -> test_utilities_utility_mem_res_mem_res_aliases_header_vector_synop2; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// TODO: Change to XFAIL once https://llvm.org/PR40995 is fixed
// UNSUPPORTED: availability-pmr-missing

// <vector>

// namespace std::pmr {
//
// typedef ... vector
//
// } // namespace std::pmr

#include <vector>

void test_utilities_utility_mem_res_mem_res_aliases_header_vector_synop2() {
  {
    // Check that std::pmr::vector is usable without <memory_resource>.
    std::pmr::vector<int> l;
  }

  return;
}
