// AST-transferred from libc++ by tools/transfer.py (slug=containers_container_adaptors_queue_queue_cons_move_noexcept).
// main -> test_containers_container_adaptors_queue_queue_cons_move_noexcept; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <queue>

// queue(queue&&)
//        noexcept(is_nothrow_move_constructible<container_type>::value);

// This tests a conforming extension

#include <queue>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"

void test_containers_container_adaptors_queue_queue_cons_move_noexcept() {
#if defined(_LIBCPP_VERSION)
  {
    typedef std::queue<MoveOnly> C;
    static_assert(std::is_nothrow_move_constructible<C>::value, "");
  }
#endif

  return;
}
