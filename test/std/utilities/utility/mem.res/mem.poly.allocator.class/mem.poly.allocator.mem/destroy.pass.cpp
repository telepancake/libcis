// AST-transferred from libc++ by tools/transfer.py (slug=utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_mem_destroy).
// main -> test_utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_mem_destroy; file-scope helpers isolated in anon namespace.
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

// template <class U>
// void polymorphic_allocator<T>::destroy(U * ptr);

#include <memory_resource>
#include <cassert>
#include <new>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_mem_destroy { // libcis: isolate file-scope helpers
int count = 0;

struct destroyable {
  destroyable() { ++count; }
  ~destroyable() { --count; }
};
} using namespace libcis_ns_utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_mem_destroy; // libcis


void test_utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_mem_destroy() {
  typedef std::pmr::polymorphic_allocator<double> A;
  {
    A a;
    ASSERT_SAME_TYPE(decltype(a.destroy((destroyable*)nullptr)), void);
  }
  {
    alignas(destroyable) char buffer[sizeof(destroyable)];
    destroyable* ptr = ::new (buffer) destroyable();
    assert(count == 1);
    A{}.destroy(ptr);
    assert(count == 0);
  }

  return;
}
