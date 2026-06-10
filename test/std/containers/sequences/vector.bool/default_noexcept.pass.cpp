// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_vector_bool_default_noexcept).
// main -> test_containers_sequences_vector_bool_default_noexcept; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03

// <vector>

// vector<bool>()
//        noexcept(is_nothrow_default_constructible<allocator_type>::value);

// This tests a conforming extension
// For vector<>, this was added to the standard by N4258,
//   but vector<bool> was not changed.

#include <cassert>
#include <vector>
#include <type_traits>

#include "test_macros.h"
#include "test_allocator.h"

namespace libcis_ns_containers_sequences_vector_bool_default_noexcept { // libcis: isolate file-scope helpers
template <class T>
struct some_alloc {
  typedef T value_type;
  some_alloc(const some_alloc&);
};
} using namespace libcis_ns_containers_sequences_vector_bool_default_noexcept; // libcis


void test_containers_sequences_vector_bool_default_noexcept() {
#if defined(_LIBCPP_VERSION)
  {
    typedef std::vector<bool> C;
    static_assert(std::is_nothrow_default_constructible<C>::value, "");
  }
  {
    typedef std::vector<bool, test_allocator<bool>> C;
    static_assert(std::is_nothrow_default_constructible<C>::value, "");
  }
  {
    typedef std::vector<bool, other_allocator<bool>> C;
    static_assert(!std::is_nothrow_default_constructible<C>::value, "");
  }
  {
    typedef std::vector<bool, some_alloc<bool>> C;
    static_assert(!std::is_nothrow_default_constructible<C>::value, "");
  }
#endif // _LIBCPP_VERSION

  return;
}
