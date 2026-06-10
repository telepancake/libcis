// AST-transferred from libc++ by tools/transfer.py (slug=utilities_utility_mem_res_mem_res_aliases_header_set_synop2).
// main -> test_utilities_utility_mem_res_mem_res_aliases_header_set_synop2; file-scope helpers isolated in anon namespace.
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

// <set>

// namespace std::pmr {
//
// typedef ... set
//
// } // namespace std::pmr

#include <set>

void test_utilities_utility_mem_res_mem_res_aliases_header_set_synop2() {
  {
    // Check that std::pmr::set is usable without <memory_resource>.
    std::pmr::set<int> s;
    std::pmr::multiset<int> ms;
  }

  return;
}
