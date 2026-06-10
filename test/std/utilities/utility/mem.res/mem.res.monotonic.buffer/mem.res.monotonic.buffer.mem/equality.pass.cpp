// AST-transferred from libc++ by tools/transfer.py (slug=utilities_utility_mem_res_mem_res_monotonic_buffer_mem_res_monotonic_buffer_mem_equality).
// main -> test_utilities_utility_mem_res_mem_res_monotonic_buffer_mem_res_monotonic_buffer_mem_equality; file-scope helpers isolated in anon namespace.
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
#include <cstddef> // size_t

namespace libcis_ns_utilities_utility_mem_res_mem_res_monotonic_buffer_mem_res_monotonic_buffer_mem_equality { // libcis: isolate file-scope helpers
struct assert_on_compare : public std::pmr::memory_resource {
  void* do_allocate(std::size_t, size_t) override {
    assert(false);
    return nullptr;
  }

  void do_deallocate(void*, std::size_t, size_t) override { assert(false); }

  bool do_is_equal(const std::pmr::memory_resource&) const noexcept override {
    assert(false);
    return true;
  }
};
} using namespace libcis_ns_utilities_utility_mem_res_mem_res_monotonic_buffer_mem_res_monotonic_buffer_mem_equality; // libcis


void test_utilities_utility_mem_res_mem_res_monotonic_buffer_mem_res_monotonic_buffer_mem_equality() {
  // Same object
  {
    std::pmr::monotonic_buffer_resource r1;
    std::pmr::monotonic_buffer_resource r2;
    assert(r1 == r1);
    assert(r1 != r2);

    std::pmr::memory_resource& p1 = r1;
    std::pmr::memory_resource& p2 = r2;
    assert(p1 == p1);
    assert(p1 != p2);
    assert(p1 == r1);
    assert(r1 == p1);
    assert(p1 != r2);
    assert(r2 != p1);
  }
  // Different types
  {
    std::pmr::monotonic_buffer_resource mono1;
    std::pmr::memory_resource& r1 = mono1;
    assert_on_compare c;
    std::pmr::memory_resource& r2 = c;
    assert(r1 != r2);
    assert(!(r1 == r2));
  }

  return;
}
