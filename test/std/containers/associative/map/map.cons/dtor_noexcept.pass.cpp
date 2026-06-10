// AST-transferred from libc++ by tools/transfer.py (slug=containers_associative_map_map_cons_dtor_noexcept).
// main -> test_containers_associative_map_map_cons_dtor_noexcept; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <map>

// ~map() // implied noexcept; constexpr since C++26

// UNSUPPORTED: c++03

#include <cassert>
#include <map>
#include <type_traits>

#include "test_macros.h"
#include "MoveOnly.h"
#include "test_allocator.h"

namespace libcis_ns_containers_associative_map_map_cons_dtor_noexcept { // libcis: isolate file-scope helpers
template <class T>
struct some_comp {
  typedef T value_type;
  ~some_comp() noexcept(false);
  bool operator()(const T&, const T&) const noexcept { return false; }
};

TEST_CONSTEXPR_CXX26 bool test() {
  typedef std::pair<const MoveOnly, MoveOnly> V;
  {
    typedef std::map<MoveOnly, MoveOnly> C;
    static_assert(std::is_nothrow_destructible<C>::value, "");
  }
  {
    typedef std::map<MoveOnly, MoveOnly, std::less<MoveOnly>, test_allocator<V>> C;
    static_assert(std::is_nothrow_destructible<C>::value, "");
  }
  {
    typedef std::map<MoveOnly, MoveOnly, std::less<MoveOnly>, other_allocator<V>> C;
    static_assert(std::is_nothrow_destructible<C>::value, "");
  }
#if defined(_LIBCPP_VERSION)
  {
    typedef std::map<MoveOnly, MoveOnly, some_comp<MoveOnly>> C;
    static_assert(!std::is_nothrow_destructible<C>::value, "");
  }
#endif // _LIBCPP_VERSION
  return true;
}
} using namespace libcis_ns_containers_associative_map_map_cons_dtor_noexcept; // libcis


void test_containers_associative_map_map_cons_dtor_noexcept() {
  test();
#if TEST_STD_VER >= 26
  static_assert(test());
#endif
  return;
}
