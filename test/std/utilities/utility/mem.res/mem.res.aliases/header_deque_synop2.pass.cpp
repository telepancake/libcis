// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_utility_mem_res_mem_res_aliases_header_deque_synop2_d29e3082).
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

// <deque>

// namespace std::pmr {
//
// typedef ... deque
//
// } // namespace std::pmr

#include <deque>

namespace libcis_ns_utilities_utility_mem_res_mem_res_aliases_header_deque_synop2_d29e3082 { // libcis
int main(int, char**) {
  {
    // Check that std::pmr::deque is usable without <memory_resource>.
    std::pmr::deque<int> d;
  }

  return 0;

    return 0;
}
} // libcis_ns_utilities_utility_mem_res_mem_res_aliases_header_deque_synop2_d29e3082 (libcis)

