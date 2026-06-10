// AST-transferred from libc++ by tools/transfer.py (slug=containers_unord_unord_multiset_unord_multiset_cnstr_move_noexcept).
// main -> test_containers_unord_unord_multiset_unord_multiset_cnstr_move_noexcept; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <unordered_set>

// unordered_multiset(unordered_multiset&&)
//        noexcept(is_nothrow_move_constructible<allocator_type>::value &&
//                 is_nothrow_move_constructible<key_compare>::value);

// This tests a conforming extension

// UNSUPPORTED: c++03

#include <unordered_set>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"
#include "test_allocator.h"

namespace libcis_ns_containers_unord_unord_multiset_unord_multiset_cnstr_move_noexcept { // libcis: isolate file-scope helpers
template <class T>
struct some_comp {
  typedef T value_type;
  some_comp(const some_comp&);
  bool operator()(const T&, const T&) const { return false; }
};

template <class T>
struct some_hash {
  typedef T value_type;
  some_hash();
  some_hash(const some_hash&);
  std::size_t operator()(T const&) const;
};
} using namespace libcis_ns_containers_unord_unord_multiset_unord_multiset_cnstr_move_noexcept; // libcis


void test_containers_unord_unord_multiset_unord_multiset_cnstr_move_noexcept() {
#if defined(_LIBCPP_VERSION)
  {
    typedef std::unordered_multiset<MoveOnly> C;
    static_assert(std::is_nothrow_move_constructible<C>::value, "");
  }
  {
    typedef std::unordered_multiset<MoveOnly, std::hash<MoveOnly>, std::equal_to<MoveOnly>, test_allocator<MoveOnly>> C;
    static_assert(std::is_nothrow_move_constructible<C>::value, "");
  }
  {
    typedef std::unordered_multiset<MoveOnly, std::hash<MoveOnly>, std::equal_to<MoveOnly>, other_allocator<MoveOnly>>
        C;
    static_assert(std::is_nothrow_move_constructible<C>::value, "");
  }
#endif // _LIBCPP_VERSION
  {
    typedef std::unordered_multiset<MoveOnly, some_hash<MoveOnly>> C;
    static_assert(!std::is_nothrow_move_constructible<C>::value, "");
  }
  {
    typedef std::unordered_multiset<MoveOnly, std::hash<MoveOnly>, some_comp<MoveOnly>> C;
    static_assert(!std::is_nothrow_move_constructible<C>::value, "");
  }

  return;
}
