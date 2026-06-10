// AST-transferred from libc++ by tools/transfer.py (slug=utilities_utility_mem_res_mem_res_aliases_header_unordered_map_synop2).
// main -> test_utilities_utility_mem_res_mem_res_aliases_header_unordered_map_synop2; file-scope helpers isolated in anon namespace.
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

// <unordered_map>

// namespace std::pmr {
//
// typedef ... unordered_map
//
// } // namespace std::pmr

#include <unordered_map>

void test_utilities_utility_mem_res_mem_res_aliases_header_unordered_map_synop2() {
  {
    // Check that std::pmr::unordered_map is usable without <memory_resource>.
    std::pmr::unordered_map<int, int> m;
    std::pmr::unordered_multimap<int, int> mm;
  }

  return;
}
