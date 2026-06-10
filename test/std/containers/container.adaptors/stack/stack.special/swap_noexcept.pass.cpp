// AST-transferred from libc++ by tools/transfer.py (slug=containers_container_adaptors_stack_stack_special_swap_noexcept).
// main -> test_containers_container_adaptors_stack_stack_special_swap_noexcept; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <stack>

// void swap(stack& c)
//     noexcept(__is_nothrow_swappable<container_type>::value);

// This tests a conforming extension

// UNSUPPORTED: c++03

#include <stack>
#include <utility>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"

void test_containers_container_adaptors_stack_stack_special_swap_noexcept() {
  {
    typedef std::stack<MoveOnly> C;
    static_assert(noexcept(swap(std::declval<C&>(), std::declval<C&>())), "");
  }

  return;
}
