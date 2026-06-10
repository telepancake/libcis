// AST-transferred from libc++ by tools/transfer.py (slug=utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_mem_select_on_container_copy_construction).
// main -> test_utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_mem_select_on_container_copy_construction; file-scope helpers isolated in anon namespace.
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

// template <class T> class polymorphic_allocator

// polymorphic_allocator
// polymorphic_allocator<T>::select_on_container_copy_construction() const

#include <cassert>
#include <cstddef>
#include <memory_resource>
#include <new>

#include "test_macros.h"

namespace libcis_ns_utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_mem_select_on_container_copy_construction { // libcis: isolate file-scope helpers
struct resource : std::pmr::memory_resource {
  void* do_allocate(size_t, size_t) override { TEST_THROW(std::bad_alloc()); }
  void do_deallocate(void*, size_t, size_t) override { assert(false); }
  bool do_is_equal(const std::pmr::memory_resource&) const noexcept override { return false; }
};
} using namespace libcis_ns_utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_mem_select_on_container_copy_construction; // libcis


void test_utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_mem_select_on_container_copy_construction() {
  typedef std::pmr::polymorphic_allocator<void> A;
  {
    A const a;
    ASSERT_SAME_TYPE(decltype(a.select_on_container_copy_construction()), A);
  }
  {
    resource res;
    A const a(&res);
    assert(a.resource() == &res);
    A const other = a.select_on_container_copy_construction();
    assert(other.resource() == std::pmr::get_default_resource());
    assert(a.resource() == &res);
  }

  return;
}
