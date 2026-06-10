// AST-transferred from libc++ by tools/transfer.py (slug=utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_mem_construct_pair).
// main -> test_utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_mem_construct_pair; file-scope helpers isolated in anon namespace.
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

// template <class U1, class U2>
// void polymorphic_allocator<T>::construct(pair<U1, U2>*)

#include <memory_resource>
#include <cassert>
#include <tuple>
#include <type_traits>
#include <utility>
#include "uses_alloc_types.h"

namespace libcis_ns_utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_mem_construct_pair { // libcis: isolate file-scope helpers
int constructed = 0;

template <int>
struct default_constructible {
  default_constructible() : x(42) { ++constructed; }
  int x = 0;
};
} using namespace libcis_ns_utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_mem_construct_pair; // libcis


void test_utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_mem_construct_pair() {
  // pair<default_constructible, default_constructible>
  {
    typedef default_constructible<0> T;
    typedef std::pair<T, T> P;
    typedef std::pmr::polymorphic_allocator<void> A;
    alignas(P) char buffer[sizeof(P)];
    P* ptr = reinterpret_cast<P*>(buffer);
    A a;
    constructed = 0;
    a.construct(ptr);
    assert(constructed == 2);
    assert(ptr->first.x == 42);
    assert(ptr->second.x == 42);
  }

  // pair<default_constructible<0>, default_constructible<1>>
  {
    typedef default_constructible<0> T;
    typedef default_constructible<1> U;
    typedef std::pair<T, U> P;
    typedef std::pmr::polymorphic_allocator<void> A;
    alignas(P) char buffer[sizeof(P)];
    P* ptr = reinterpret_cast<P*>(buffer);
    A a;
    constructed = 0;
    a.construct(ptr);
    assert(constructed == 2);
    assert(ptr->first.x == 42);
    assert(ptr->second.x == 42);
  }

  return;
}
