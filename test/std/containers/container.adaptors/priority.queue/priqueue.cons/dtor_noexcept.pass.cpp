// transferred+adapted from libc++ by tools/transfer.py (slug=containers_container_adaptors_priority_queue_priqueue_cons_dtor_noexcept_8203fa63).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <queue>

// ~priority_queue() // implied noexcept;

// UNSUPPORTED: c++03

#include <queue>
#include <cassert>
#include <type_traits>

#include "test_macros.h"
#include "MoveOnly.h"

namespace libcis_ns_containers_container_adaptors_priority_queue_priqueue_cons_dtor_noexcept_8203fa63 { // libcis
int main(int, char**)
{
    {
        typedef std::priority_queue<MoveOnly> C;
        static_assert(std::is_nothrow_destructible<C>::value, "");
    }

  return 0;

    return 0;
}
} // libcis_ns_containers_container_adaptors_priority_queue_priqueue_cons_dtor_noexcept_8203fa63 (libcis)

