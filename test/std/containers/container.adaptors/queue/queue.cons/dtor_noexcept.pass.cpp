// AST-transferred from libc++ by tools/transfer.py (slug=containers_container_adaptors_queue_queue_cons_dtor_noexcept).
// main -> test_containers_container_adaptors_queue_queue_cons_dtor_noexcept; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <queue>

// ~queue() // implied noexcept;

// UNSUPPORTED: c++03

#include <queue>
#include <cassert>
#include <type_traits>

#include "test_macros.h"
#include "MoveOnly.h"

void test_containers_container_adaptors_queue_queue_cons_dtor_noexcept() {
  {
    typedef std::queue<MoveOnly> C;
    static_assert(std::is_nothrow_destructible<C>::value, "");
  }

  return;
}
