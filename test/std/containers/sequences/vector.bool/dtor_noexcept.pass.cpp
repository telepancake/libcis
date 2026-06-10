// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_vector_bool_dtor_noexcept).
// main -> test_containers_sequences_vector_bool_dtor_noexcept; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <vector>

// ~vector<bool>() // implied noexcept;

// UNSUPPORTED: c++03

#include <cassert>
#include <vector>
#include <type_traits>

#include "test_macros.h"
#include "test_allocator.h"

namespace libcis_ns_containers_sequences_vector_bool_dtor_noexcept { // libcis: isolate file-scope helpers
template <class T>
struct some_alloc {
  typedef T value_type;
  some_alloc(const some_alloc&);
  ~some_alloc() noexcept(false);
};
} using namespace libcis_ns_containers_sequences_vector_bool_dtor_noexcept; // libcis


void test_containers_sequences_vector_bool_dtor_noexcept() {
  {
    typedef std::vector<bool> C;
    static_assert(std::is_nothrow_destructible<C>::value, "");
  }
  {
    typedef std::vector<bool, test_allocator<bool>> C;
    static_assert(std::is_nothrow_destructible<C>::value, "");
  }
  {
    typedef std::vector<bool, other_allocator<bool>> C;
    static_assert(std::is_nothrow_destructible<C>::value, "");
  }
#if defined(_LIBCPP_VERSION)
  {
    typedef std::vector<bool, some_alloc<bool>> C;
    static_assert(!std::is_nothrow_destructible<C>::value, "");
  }
#endif // _LIBCPP_VERSION

  return;
}
