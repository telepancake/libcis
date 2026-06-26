// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_utility_mem_res_mem_res_aliases_header_vector_synop2_11eeb416).
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

// <vector>

// namespace std::pmr {
//
// typedef ... vector
//
// } // namespace std::pmr

#include <vector>

namespace libcis_ns_utilities_utility_mem_res_mem_res_aliases_header_vector_synop2_11eeb416 { // libcis
int main(int, char**) {
  {
    // Check that std::pmr::vector is usable without <memory_resource>.
    std::pmr::vector<int> l;
  }

  return 0;

    return 0;
}
} // libcis_ns_utilities_utility_mem_res_mem_res_aliases_header_vector_synop2_11eeb416 (libcis)

