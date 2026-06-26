// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_utility_mem_res_mem_res_monotonic_buffer_mem_res_monotonic_buffer_mem_allocate_from_zero_sized_buffer_b54bc367).
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

// <memory_resource>

// class monotonic_buffer_resource

#include <memory_resource>
#include <cassert>

#include "count_new.h"
#include "test_macros.h"

namespace libcis_ns_utilities_utility_mem_res_mem_res_monotonic_buffer_mem_res_monotonic_buffer_mem_allocate_from_zero_sized_buffer_b54bc367 { // libcis
int main(int, char**) {
  globalMemCounter.reset();
  {
    char buffer[100];
    auto mono1                    = std::pmr::monotonic_buffer_resource(buffer, 0, std::pmr::new_delete_resource());
    std::pmr::memory_resource& r1 = mono1;

    void* ret = r1.allocate(1, 1);
    assert(ret != nullptr);
    ASSERT_WITH_LIBRARY_INTERNAL_ALLOCATIONS(globalMemCounter.checkNewCalledEq(1));
  }
  ASSERT_WITH_LIBRARY_INTERNAL_ALLOCATIONS(globalMemCounter.checkDeleteCalledEq(1));
  assert(globalMemCounter.checkOutstandingNewEq(0));

  globalMemCounter.reset();
  {
    auto mono1                    = std::pmr::monotonic_buffer_resource(nullptr, 0, std::pmr::new_delete_resource());
    std::pmr::memory_resource& r1 = mono1;

    void* ret = r1.allocate(1, 1);
    assert(ret != nullptr);
    ASSERT_WITH_LIBRARY_INTERNAL_ALLOCATIONS(globalMemCounter.checkNewCalledEq(1));
  }
  ASSERT_WITH_LIBRARY_INTERNAL_ALLOCATIONS(globalMemCounter.checkDeleteCalledEq(1));
  assert(globalMemCounter.checkOutstandingNewEq(0));

  return 0;

    return 0;
}
} // libcis_ns_utilities_utility_mem_res_mem_res_monotonic_buffer_mem_res_monotonic_buffer_mem_allocate_from_zero_sized_buffer_b54bc367 (libcis)

