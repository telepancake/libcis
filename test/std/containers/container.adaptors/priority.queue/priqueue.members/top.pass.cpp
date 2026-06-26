// transferred+adapted from libc++ by tools/transfer.py (slug=containers_container_adaptors_priority_queue_priqueue_members_top_566b574a).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <queue>

// priority_queue();

// const_reference top() const;

#include <queue>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_containers_container_adaptors_priority_queue_priqueue_members_top_566b574a { // libcis
int main(int, char**)
{
    std::priority_queue<int> q;
    q.push(1);
    assert(q.top() == 1);
    q.push(3);
    assert(q.top() == 3);
    q.push(2);
    assert(q.top() == 3);

  return 0;

    return 0;
}
} // libcis_ns_containers_container_adaptors_priority_queue_priqueue_members_top_566b574a (libcis)

