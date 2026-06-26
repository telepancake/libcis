// transferred+adapted from libc++ by tools/transfer.py (slug=containers_container_adaptors_stack_stack_defn_top_cc83004c).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <stack>

// reference top();

#include <stack>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_containers_container_adaptors_stack_stack_defn_top_cc83004c { // libcis
int main(int, char**)
{
    std::stack<int> q;
    assert(q.size() == 0);
    q.push(1);
    q.push(2);
    q.push(3);
    int& ir = q.top();
    assert(ir == 3);

  return 0;

    return 0;
}
} // libcis_ns_containers_container_adaptors_stack_stack_defn_top_cc83004c (libcis)

