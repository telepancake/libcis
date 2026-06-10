// AST-transferred from libc++ by tools/transfer.py (slug=containers_associative_set_set_cons_dtor_noexcept).
// main -> test_containers_associative_set_set_cons_dtor_noexcept; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <set>

// ~set() // implied noexcept;

// UNSUPPORTED: c++03

#include <cassert>
#include <set>
#include <type_traits>

#include "test_macros.h"
#include "MoveOnly.h"
#include "test_allocator.h"

namespace libcis_ns_containers_associative_set_set_cons_dtor_noexcept { // libcis: isolate file-scope helpers
template <class T>
struct some_comp {
  typedef T value_type;
  ~some_comp() noexcept(false);
  bool operator()(const T&, const T&) const { return false; }
};
} using namespace libcis_ns_containers_associative_set_set_cons_dtor_noexcept; // libcis


void test_containers_associative_set_set_cons_dtor_noexcept() {
  {
    typedef std::set<MoveOnly> C;
    static_assert(std::is_nothrow_destructible<C>::value, "");
  }
  {
    typedef std::set<MoveOnly, std::less<MoveOnly>, test_allocator<MoveOnly>> C;
    static_assert(std::is_nothrow_destructible<C>::value, "");
  }
  {
    typedef std::set<MoveOnly, std::less<MoveOnly>, other_allocator<MoveOnly>> C;
    static_assert(std::is_nothrow_destructible<C>::value, "");
  }
#if defined(_LIBCPP_VERSION)
  {
    typedef std::set<MoveOnly, some_comp<MoveOnly>> C;
    static_assert(!std::is_nothrow_destructible<C>::value, "");
  }
#endif // _LIBCPP_VERSION

  return;
}
