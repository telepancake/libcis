// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_mem_resource_afc12b64).
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

// template <class T> class polymorphic_allocator

// memory_resource *
// polymorphic_allocator<T>::resource() const

#include <memory_resource>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_mem_resource_afc12b64 { // libcis
int main(int, char**) {
  typedef std::pmr::polymorphic_allocator<void> A;
  {
    A const a;
    ASSERT_SAME_TYPE(decltype(a.resource()), std::pmr::memory_resource*);
  }
  {
    std::pmr::memory_resource* mptr = (std::pmr::memory_resource*)42;
    A const a(mptr);
    assert(a.resource() == mptr);
  }
  {
    A const a(nullptr);
    assert(a.resource() == nullptr);
    assert(a.resource() == nullptr);
  }
  {
    A const a;
    assert(a.resource() == std::pmr::get_default_resource());
  }
  {
    std::pmr::memory_resource* mptr = (std::pmr::memory_resource*)42;
    std::pmr::set_default_resource(mptr);
    A const a;
    assert(a.resource() == mptr);
    assert(a.resource() == std::pmr::get_default_resource());
  }

  return 0;

    return 0;
}
} // libcis_ns_utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_mem_resource_afc12b64 (libcis)

