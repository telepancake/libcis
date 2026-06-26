// transferred+adapted from libc++ by tools/transfer.py (slug=containers_container_adaptors_stack_stack_cons_move_assign_noexcept_efb54e4d).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <stack>

// stack& operator=(stack&& c)
//     noexcept(is_nothrow_move_assignable<container_type>::value);

// This tests a conforming extension

// UNSUPPORTED: c++03

#include <stack>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"

namespace libcis_ns_containers_container_adaptors_stack_stack_cons_move_assign_noexcept_efb54e4d { // libcis
int main(int, char**)
{
    {
        typedef std::stack<MoveOnly> C;
        static_assert(std::is_nothrow_move_assignable<C>::value, "");
    }

  return 0;

    return 0;
}
} // libcis_ns_containers_container_adaptors_stack_stack_cons_move_assign_noexcept_efb54e4d (libcis)

