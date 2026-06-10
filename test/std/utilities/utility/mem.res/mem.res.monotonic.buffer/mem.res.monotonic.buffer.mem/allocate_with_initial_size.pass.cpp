// AST-transferred from libc++ by tools/transfer.py (slug=utilities_utility_mem_res_mem_res_monotonic_buffer_mem_res_monotonic_buffer_mem_allocate_with_initial_size).
// main -> test_utilities_utility_mem_res_mem_res_monotonic_buffer_mem_res_monotonic_buffer_mem_allocate_with_initial_size; file-scope helpers isolated in anon namespace.
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

// <memory_resource>

// class monotonic_buffer_resource

#include <memory_resource>
#include <cassert>

#include "count_new.h"
#include "test_macros.h"

namespace libcis_ns_utilities_utility_mem_res_mem_res_monotonic_buffer_mem_res_monotonic_buffer_mem_allocate_with_initial_size { // libcis: isolate file-scope helpers
void test(std::size_t initial_buffer_size) {
  globalMemCounter.reset();

  auto mono1 = std::pmr::monotonic_buffer_resource(initial_buffer_size, std::pmr::new_delete_resource());
  assert(globalMemCounter.checkNewCalledEq(0));

  (void)mono1.allocate(1, 1);
  ASSERT_WITH_LIBRARY_INTERNAL_ALLOCATIONS(globalMemCounter.checkNewCalledEq(1));
  ASSERT_WITH_LIBRARY_INTERNAL_ALLOCATIONS(globalMemCounter.checkLastNewSizeGe(initial_buffer_size));

  (void)mono1.allocate(initial_buffer_size - 1, 1);
  ASSERT_WITH_LIBRARY_INTERNAL_ALLOCATIONS(globalMemCounter.checkNewCalledEq(1));
}
} using namespace libcis_ns_utilities_utility_mem_res_mem_res_monotonic_buffer_mem_res_monotonic_buffer_mem_allocate_with_initial_size; // libcis


void test_utilities_utility_mem_res_mem_res_monotonic_buffer_mem_res_monotonic_buffer_mem_allocate_with_initial_size() {
  test(1);
  test(8);
  test(10);
  test(100);
  test(256);
  test(1000);
  test(1024);
  test(1000000);
  assert(globalMemCounter.checkOutstandingNewEq(0));

  return;
}
