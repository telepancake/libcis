// transferred+adapted from libc++ by tools/transfer.py (slug=containers_container_adaptors_stack_stack_defn_top_const_a4a1e273).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <stack>

// const_reference top() const;

#include <stack>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_containers_container_adaptors_stack_stack_defn_top_const_a4a1e273 { // libcis
int main(int, char**)
{
    std::stack<int> q;
    assert(q.size() == 0);
    q.push(1);
    q.push(2);
    q.push(3);
    const std::stack<int>& cqr = q;
    const int& cir = cqr.top();
    assert(cir == 3);

  return 0;

    return 0;
}
} // libcis_ns_containers_container_adaptors_stack_stack_defn_top_const_a4a1e273 (libcis)

