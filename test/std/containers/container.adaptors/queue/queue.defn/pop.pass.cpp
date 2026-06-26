// transferred+adapted from libc++ by tools/transfer.py (slug=containers_container_adaptors_queue_queue_defn_pop_a6c56f39).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <queue>

// void pop();

#include <queue>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_containers_container_adaptors_queue_queue_defn_pop_a6c56f39 { // libcis
int main(int, char**)
{
    std::queue<int> q;
    assert(q.size() == 0);
    q.push(1);
    q.push(2);
    q.push(3);
    assert(q.size() == 3);
    assert(q.front() == 1);
    assert(q.back() == 3);
    q.pop();
    assert(q.size() == 2);
    assert(q.front() == 2);
    assert(q.back() == 3);
    q.pop();
    assert(q.size() == 1);
    assert(q.front() == 3);
    assert(q.back() == 3);
    q.pop();
    assert(q.size() == 0);

  return 0;

    return 0;
}
} // libcis_ns_containers_container_adaptors_queue_queue_defn_pop_a6c56f39 (libcis)

