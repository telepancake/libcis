// AST-transferred from libc++ by tools/transfer.py (slug=containers_associative_map_map_cons_default_noexcept).
// main -> test_containers_associative_map_map_cons_default_noexcept; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <map>

// map()
//    noexcept(
//        is_nothrow_default_constructible<allocator_type>::value &&
//        is_nothrow_default_constructible<key_compare>::value &&
//        is_nothrow_copy_constructible<key_compare>::value); // constexpr since C++26

// This tests a conforming extension

// UNSUPPORTED: c++03

#include <map>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"
#include "test_allocator.h"

namespace libcis_ns_containers_associative_map_map_cons_default_noexcept { // libcis: isolate file-scope helpers
template <class T>
struct some_comp {
  typedef T value_type;
  some_comp();
  bool operator()(const T&, const T&) const { return false; }
};

TEST_CONSTEXPR_CXX26 bool test() {
  typedef std::pair<const MoveOnly, MoveOnly> V;
#if defined(_LIBCPP_VERSION)
  {
    typedef std::map<MoveOnly, MoveOnly> C;
    static_assert(std::is_nothrow_default_constructible<C>::value, "");
  }
  {
    typedef std::map<MoveOnly, MoveOnly, std::less<MoveOnly>, test_allocator<V>> C;
    static_assert(std::is_nothrow_default_constructible<C>::value, "");
  }
#endif // _LIBCPP_VERSION
  {
    typedef std::map<MoveOnly, MoveOnly, std::less<MoveOnly>, other_allocator<V>> C;
    static_assert(!std::is_nothrow_default_constructible<C>::value, "");
  }
  {
    typedef std::map<MoveOnly, MoveOnly, some_comp<MoveOnly>> C;
    static_assert(!std::is_nothrow_default_constructible<C>::value, "");
  }
  return true;
}
} using namespace libcis_ns_containers_associative_map_map_cons_default_noexcept; // libcis


void test_containers_associative_map_map_cons_default_noexcept() {
  test();
#if TEST_STD_VER >= 26
  static_assert(test());
#endif
  return;
}
