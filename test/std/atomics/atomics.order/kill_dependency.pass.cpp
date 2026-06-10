// AST-transferred from libc++ by tools/transfer.py (slug=atomics_atomics_order_kill_dependency).
// main -> test_atomics_atomics_order_kill_dependency; file-scope helpers isolated in anon namespace.
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

void test_atomics_atomics_order_kill_dependency()
{
    assert(std::kill_dependency(5) == 5);
    assert(std::kill_dependency(-5.5) == -5.5);

  return;
}
