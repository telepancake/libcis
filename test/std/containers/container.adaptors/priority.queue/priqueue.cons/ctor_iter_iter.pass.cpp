// transferred+adapted from libc++ by tools/transfer.py (slug=containers_container_adaptors_priority_queue_priqueue_cons_ctor_iter_iter_c238684e).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <queue>

// template <class InputIterator>
//   priority_queue(InputIterator first, InputIterator last);

#include <queue>
#include <cassert>
#include <cstddef>

#include "test_macros.h"

namespace libcis_ns_containers_container_adaptors_priority_queue_priqueue_cons_ctor_iter_iter_c238684e { // libcis
int main(int, char**)
{
    int a[] = {3, 5, 2, 0, 6, 8, 1};
    int* an = a + sizeof(a)/sizeof(a[0]);
    std::priority_queue<int> q(a, an);
    assert(q.size() == static_cast<std::size_t>(an - a));
    assert(q.top() == 8);

  return 0;

    return 0;
}
} // libcis_ns_containers_container_adaptors_priority_queue_priqueue_cons_ctor_iter_iter_c238684e (libcis)

