// transferred+adapted from libc++ by tools/transfer.py (slug=containers_container_adaptors_stack_stack_cons_move_noexcept_0bb6ca21).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <stack>

// stack(stack&&)
//        noexcept(is_nothrow_move_constructible<container_type>::value);

// This tests a conforming extension

#include <stack>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"

namespace libcis_ns_containers_container_adaptors_stack_stack_cons_move_noexcept_0bb6ca21 { // libcis
int main(int, char**)
{
#if defined(_LIBCPP_VERSION)
    {
        typedef std::stack<MoveOnly> C;
        static_assert(std::is_nothrow_move_constructible<C>::value, "");
    }
#endif

  return 0;

    return 0;
}
} // libcis_ns_containers_container_adaptors_stack_stack_cons_move_noexcept_0bb6ca21 (libcis)

