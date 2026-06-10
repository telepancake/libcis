// AST-transferred from libc++ by tools/transfer.py (slug=utilities_utility_mem_res_mem_res_pool_mem_res_pool_mem_unsync_allocate_reuse_blocks).
// main -> test_utilities_utility_mem_res_mem_res_pool_mem_res_pool_mem_unsync_allocate_reuse_blocks; file-scope helpers isolated in anon namespace.
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

// class unsynchronized_pool_resource

#include <memory_resource>
#include <cassert>
#include <memory> // std::align

#include "count_new.h"
#include "test_macros.h"

namespace libcis_ns_utilities_utility_mem_res_mem_res_pool_mem_res_pool_mem_unsync_allocate_reuse_blocks { // libcis: isolate file-scope helpers
static bool is_aligned_to(void* p, std::size_t alignment) {
  void* p2     = p;
  std::size_t space = 1;
  void* result = std::align(alignment, 1, p2, space);
  return (result == p);
}
} using namespace libcis_ns_utilities_utility_mem_res_mem_res_pool_mem_res_pool_mem_unsync_allocate_reuse_blocks; // libcis


void test_utilities_utility_mem_res_mem_res_pool_mem_res_pool_mem_unsync_allocate_reuse_blocks() {
  globalMemCounter.reset();
  std::pmr::pool_options opts{1, 256};
  auto unsync1                  = std::pmr::unsynchronized_pool_resource(opts, std::pmr::new_delete_resource());
  std::pmr::memory_resource& r1 = unsync1;

  void* ret = r1.allocate(8);
  assert(ret != nullptr);
  assert(is_aligned_to(ret, 8));
  ASSERT_WITH_LIBRARY_INTERNAL_ALLOCATIONS(globalMemCounter.checkNewCalledGreaterThan(0));
  int new_called = globalMemCounter.new_called;

  // After deallocation, the pool for 8-byte blocks should have at least one vacancy.
  r1.deallocate(ret, 8);
  assert(globalMemCounter.new_called == new_called);
  assert(globalMemCounter.checkDeleteCalledEq(0));

  // This should return an existing block from the pool: no new allocations.
  ret = r1.allocate(8);
  assert(ret != nullptr);
  assert(is_aligned_to(ret, 8));
  assert(globalMemCounter.new_called == new_called);
  assert(globalMemCounter.checkDeleteCalledEq(0));

  return;
}
