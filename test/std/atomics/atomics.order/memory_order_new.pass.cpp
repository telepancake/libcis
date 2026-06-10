// AST-transferred from libc++ by tools/transfer.py (slug=atomics_atomics_order_memory_order_new).
// main -> test_atomics_atomics_order_memory_order_new; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

#include <atomic>

#include "test_macros.h"

void test_atomics_atomics_order_memory_order_new()
{
    static_assert(std::memory_order_relaxed == std::memory_order::relaxed);
    static_assert(std::memory_order_consume == std::memory_order::consume);
    static_assert(std::memory_order_acquire == std::memory_order::acquire);
    static_assert(std::memory_order_release == std::memory_order::release);
    static_assert(std::memory_order_acq_rel == std::memory_order::acq_rel);
    static_assert(std::memory_order_seq_cst == std::memory_order::seq_cst);

    return;
}
