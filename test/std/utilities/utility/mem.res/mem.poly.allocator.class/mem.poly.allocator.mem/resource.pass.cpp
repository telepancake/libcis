// AST-transferred from libc++ by tools/transfer.py (slug=utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_mem_resource).
// main -> test_utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_mem_resource; file-scope helpers isolated in anon namespace.
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

// memory_resource *
// polymorphic_allocator<T>::resource() const

#include <cassert>
#include <cstddef>
#include <memory_resource>
#include <new>

#include "test_macros.h"

namespace libcis_ns_utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_mem_resource { // libcis: isolate file-scope helpers
struct resource : std::pmr::memory_resource {
  void* do_allocate(size_t, size_t) override { TEST_THROW(std::bad_alloc()); }
  void do_deallocate(void*, size_t, size_t) override { assert(false); }
  bool do_is_equal(const std::pmr::memory_resource&) const noexcept override { return false; }
};
} using namespace libcis_ns_utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_mem_resource; // libcis


void test_utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_mem_resource() {
  typedef std::pmr::polymorphic_allocator<void> A;
  {
    A const a;
    ASSERT_SAME_TYPE(decltype(a.resource()), std::pmr::memory_resource*);
  }
  {
    resource res;
    A const a(&res);
    assert(a.resource() == &res);
  }
  {
    A const a;
    assert(a.resource() == std::pmr::get_default_resource());
  }
  {
    resource res;
    std::pmr::set_default_resource(&res);
    A const a;
    assert(a.resource() == &res);
    assert(a.resource() == std::pmr::get_default_resource());
  }

  return;
}
