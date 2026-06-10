// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_deque_deque_cons_move_noexcept).
// main -> test_containers_sequences_deque_deque_cons_move_noexcept; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <deque>

// deque(deque&&)
//        noexcept(is_nothrow_move_constructible<allocator_type>::value);

// This tests a conforming extension

// UNSUPPORTED: c++03

#include <deque>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"
#include "test_allocator.h"

namespace libcis_ns_containers_sequences_deque_deque_cons_move_noexcept { // libcis: isolate file-scope helpers
template <class T>
struct some_alloc {
  typedef T value_type;
  some_alloc(const some_alloc&);
  void allocate(std::size_t);
};
} using namespace libcis_ns_containers_sequences_deque_deque_cons_move_noexcept; // libcis


void test_containers_sequences_deque_deque_cons_move_noexcept() {
#if defined(_LIBCPP_VERSION)
  {
    typedef std::deque<MoveOnly> C;
    static_assert(std::is_nothrow_move_constructible<C>::value, "");
  }
  {
    typedef std::deque<MoveOnly, test_allocator<MoveOnly>> C;
    static_assert(std::is_nothrow_move_constructible<C>::value, "");
  }
  {
    typedef std::deque<MoveOnly, other_allocator<MoveOnly>> C;
    static_assert(std::is_nothrow_move_constructible<C>::value, "");
  }
  {
    typedef std::deque<MoveOnly, some_alloc<MoveOnly>> C;
    static_assert(!std::is_nothrow_move_constructible<C>::value, "");
  }
#endif // _LIBCPP_VERSION

  return;
}
