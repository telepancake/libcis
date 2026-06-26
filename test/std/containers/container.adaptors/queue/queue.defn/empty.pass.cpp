// transferred+adapted from libc++ by tools/transfer.py (slug=containers_container_adaptors_queue_queue_defn_empty_5cf84187).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <queue>

// bool empty() const;

#include <queue>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_containers_container_adaptors_queue_queue_defn_empty_5cf84187 { // libcis
int main(int, char**)
{
    std::queue<int> q;
    assert(q.empty());
    q.push(1);
    assert(!q.empty());
    q.pop();
    assert(q.empty());

  return 0;

    return 0;
}
} // libcis_ns_containers_container_adaptors_queue_queue_defn_empty_5cf84187 (libcis)

