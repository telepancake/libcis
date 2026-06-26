// transferred+adapted from libc++ by tools/transfer.py (slug=atomics_atomics_flag_atomic_flag_clear_explicit_224c3865).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <atomic>

// struct atomic_flag

// void atomic_flag_clear_explicit(volatile atomic_flag*, memory_order);
// void atomic_flag_clear_explicit(atomic_flag*, memory_order);

#include <atomic>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_atomics_atomics_flag_atomic_flag_clear_explicit_224c3865 { // libcis
int main(int, char**)
{
    {
        std::atomic_flag f; // uninitialized first
        std::atomic_flag_clear_explicit(&f, std::memory_order_relaxed);
        assert(f.test_and_set() == 0);
        std::atomic_flag_clear_explicit(&f, std::memory_order_relaxed);
        assert(f.test_and_set() == 0);
    }
    {
        std::atomic_flag f;
        std::atomic_flag_clear_explicit(&f, std::memory_order_release);
        assert(f.test_and_set() == 0);
        std::atomic_flag_clear_explicit(&f, std::memory_order_release);
        assert(f.test_and_set() == 0);
    }
    {
        std::atomic_flag f;
        std::atomic_flag_clear_explicit(&f, std::memory_order_seq_cst);
        assert(f.test_and_set() == 0);
        std::atomic_flag_clear_explicit(&f, std::memory_order_seq_cst);
        assert(f.test_and_set() == 0);
    }
    {
        volatile std::atomic_flag f;
        std::atomic_flag_clear_explicit(&f, std::memory_order_relaxed);
        assert(f.test_and_set() == 0);
        std::atomic_flag_clear_explicit(&f, std::memory_order_relaxed);
        assert(f.test_and_set() == 0);
    }
    {
        volatile std::atomic_flag f;
        std::atomic_flag_clear_explicit(&f, std::memory_order_release);
        assert(f.test_and_set() == 0);
        std::atomic_flag_clear_explicit(&f, std::memory_order_release);
        assert(f.test_and_set() == 0);
    }
    {
        volatile std::atomic_flag f;
        std::atomic_flag_clear_explicit(&f, std::memory_order_seq_cst);
        assert(f.test_and_set() == 0);
        std::atomic_flag_clear_explicit(&f, std::memory_order_seq_cst);
        assert(f.test_and_set() == 0);
    }

  return 0;

    return 0;
}
} // libcis_ns_atomics_atomics_flag_atomic_flag_clear_explicit_224c3865 (libcis)

