// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_utility_mem_res_mem_res_aliases_header_unordered_map_synop2_bb857f9a).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// TODO: Change to XFAIL once https://github.com/llvm/llvm-project/issues/40340 is fixed
// UNSUPPORTED: availability-pmr-missing

// <unordered_map>

// namespace std::pmr {
//
// typedef ... unordered_map
//
// } // namespace std::pmr

#include <unordered_map>

namespace libcis_ns_utilities_utility_mem_res_mem_res_aliases_header_unordered_map_synop2_bb857f9a { // libcis
int main(int, char**) {
  {
    // Check that std::pmr::unordered_map is usable without <memory_resource>.
    std::pmr::unordered_map<int, int> m;
    std::pmr::unordered_multimap<int, int> mm;
  }

  return 0;

    return 0;
}
} // libcis_ns_utilities_utility_mem_res_mem_res_aliases_header_unordered_map_synop2_bb857f9a (libcis)

