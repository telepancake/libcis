// transferred+adapted from libc++ by tools/transfer.py (slug=containers_container_adaptors_priority_queue_priqueue_members_size_db3e01a2).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <queue>

// priority_queue();

// size_type size() const;

#include <queue>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_containers_container_adaptors_priority_queue_priqueue_members_size_db3e01a2 { // libcis
int main(int, char**)
{
    std::priority_queue<int> q;
    assert(q.size() == 0);
    q.push(1);
    assert(q.size() == 1);
    q.pop();
    assert(q.size() == 0);

  return 0;

    return 0;
}
} // libcis_ns_containers_container_adaptors_priority_queue_priqueue_members_size_db3e01a2 (libcis)

