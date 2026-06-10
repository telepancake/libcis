// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_forwardlist_forwardlist_cons_dtor_noexcept).
// main -> test_containers_sequences_forwardlist_forwardlist_cons_dtor_noexcept; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <forward_list>

// ~forward_list() // implied noexcept;

// UNSUPPORTED: c++03

#include <cassert>
#include <forward_list>
#include <type_traits>

#include "test_macros.h"
#include "MoveOnly.h"
#include "test_allocator.h"

namespace libcis_ns_containers_sequences_forwardlist_forwardlist_cons_dtor_noexcept { // libcis: isolate file-scope helpers
template <class T>
struct some_alloc {
  typedef T value_type;
  some_alloc(const some_alloc&);
  ~some_alloc() noexcept(false);
  void allocate(std::size_t);
};
} using namespace libcis_ns_containers_sequences_forwardlist_forwardlist_cons_dtor_noexcept; // libcis


void test_containers_sequences_forwardlist_forwardlist_cons_dtor_noexcept() {
  {
    typedef std::forward_list<MoveOnly> C;
    static_assert(std::is_nothrow_destructible<C>::value, "");
  }
  {
    typedef std::forward_list<MoveOnly, test_allocator<MoveOnly>> C;
    static_assert(std::is_nothrow_destructible<C>::value, "");
  }
  {
    typedef std::forward_list<MoveOnly, other_allocator<MoveOnly>> C;
    static_assert(std::is_nothrow_destructible<C>::value, "");
  }
#if defined(_LIBCPP_VERSION)
  {
    typedef std::forward_list<MoveOnly, some_alloc<MoveOnly>> C;
    static_assert(!std::is_nothrow_destructible<C>::value, "");
  }
#endif // _LIBCPP_VERSION

  return;
}
