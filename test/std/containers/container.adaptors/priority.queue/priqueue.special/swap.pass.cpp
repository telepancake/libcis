// transferred+adapted from libc++ by tools/transfer.py (slug=containers_container_adaptors_priority_queue_priqueue_special_swap_2cab7c59).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <queue>

// priority_queue();

// template <class T, class Container, class Compare>
//   void swap(priority_queue<T, Container, Compare>& x,
//             priority_queue<T, Container, Compare>& y);

#include <queue>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_containers_container_adaptors_priority_queue_priqueue_special_swap_2cab7c59 { // libcis
int main(int, char**)
{
    std::priority_queue<int> q1;
    std::priority_queue<int> q2;
    q1.push(1);
    q1.push(3);
    q1.push(2);
    swap(q1, q2);
    assert(q1.empty());
    assert(q2.size() == 3);
    assert(q2.top() == 3);

  return 0;

    return 0;
}
} // libcis_ns_containers_container_adaptors_priority_queue_priqueue_special_swap_2cab7c59 (libcis)

