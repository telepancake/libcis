// transferred+adapted from libc++ by tools/transfer.py (slug=atomics_atomics_order_kill_dependency_04c3df41).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <atomic>

// template <class T> T kill_dependency(T y);

#include <atomic>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_atomics_atomics_order_kill_dependency_04c3df41 { // libcis
int main(int, char**)
{
    assert(std::kill_dependency(5) == 5);
    assert(std::kill_dependency(-5.5) == -5.5);

  return 0;

    return 0;
}
} // libcis_ns_atomics_atomics_order_kill_dependency_04c3df41 (libcis)

