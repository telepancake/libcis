// transferred+adapted from libc++ by tools/transfer.py (slug=atomics_atomics_fences_atomic_thread_fence_92b4cba8).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <atomic>

// void atomic_thread_fence(memory_order m);

#include <atomic>

#include "test_macros.h"

namespace libcis_ns_atomics_atomics_fences_atomic_thread_fence_92b4cba8 { // libcis
int main(int, char**)
{
    std::atomic_thread_fence(std::memory_order_seq_cst);

  return 0;

    return 0;
}
} // libcis_ns_atomics_atomics_fences_atomic_thread_fence_92b4cba8 (libcis)

