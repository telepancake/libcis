// transferred+adapted from libc++ by tools/transfer.py (slug=atomics_atomics_flag_test_and_set_151e6abb).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <atomic>

// struct atomic_flag

// bool test_and_set(memory_order = memory_order_seq_cst);
// bool test_and_set(memory_order = memory_order_seq_cst) volatile;

#include <atomic>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_atomics_atomics_flag_test_and_set_151e6abb { // libcis
int main(int, char**)
{
    {
        std::atomic_flag f;
        f.clear();
        assert(f.test_and_set() == 0);
        assert(f.test_and_set() == 1);
    }
    {
        std::atomic_flag f;
        f.clear();
        assert(f.test_and_set(std::memory_order_relaxed) == 0);
        assert(f.test_and_set(std::memory_order_relaxed) == 1);
    }
    {
        std::atomic_flag f;
        f.clear();
        assert(f.test_and_set(std::memory_order_consume) == 0);
        assert(f.test_and_set(std::memory_order_consume) == 1);
    }
    {
        std::atomic_flag f;
        f.clear();
        assert(f.test_and_set(std::memory_order_acquire) == 0);
        assert(f.test_and_set(std::memory_order_acquire) == 1);
    }
    {
        std::atomic_flag f;
        f.clear();
        assert(f.test_and_set(std::memory_order_release) == 0);
        assert(f.test_and_set(std::memory_order_release) == 1);
    }
    {
        std::atomic_flag f;
        f.clear();
        assert(f.test_and_set(std::memory_order_acq_rel) == 0);
        assert(f.test_and_set(std::memory_order_acq_rel) == 1);
    }
    {
        std::atomic_flag f;
        f.clear();
        assert(f.test_and_set(std::memory_order_seq_cst) == 0);
        assert(f.test_and_set(std::memory_order_seq_cst) == 1);
    }
    {
        volatile std::atomic_flag f;
        f.clear();
        assert(f.test_and_set() == 0);
        assert(f.test_and_set() == 1);
    }
    {
        volatile std::atomic_flag f;
        f.clear();
        assert(f.test_and_set(std::memory_order_relaxed) == 0);
        assert(f.test_and_set(std::memory_order_relaxed) == 1);
    }
    {
        volatile std::atomic_flag f;
        f.clear();
        assert(f.test_and_set(std::memory_order_consume) == 0);
        assert(f.test_and_set(std::memory_order_consume) == 1);
    }
    {
        volatile std::atomic_flag f;
        f.clear();
        assert(f.test_and_set(std::memory_order_acquire) == 0);
        assert(f.test_and_set(std::memory_order_acquire) == 1);
    }
    {
        volatile std::atomic_flag f;
        f.clear();
        assert(f.test_and_set(std::memory_order_release) == 0);
        assert(f.test_and_set(std::memory_order_release) == 1);
    }
    {
        volatile std::atomic_flag f;
        f.clear();
        assert(f.test_and_set(std::memory_order_acq_rel) == 0);
        assert(f.test_and_set(std::memory_order_acq_rel) == 1);
    }
    {
        volatile std::atomic_flag f;
        f.clear();
        assert(f.test_and_set(std::memory_order_seq_cst) == 0);
        assert(f.test_and_set(std::memory_order_seq_cst) == 1);
    }

  return 0;

    return 0;
}
} // libcis_ns_atomics_atomics_flag_test_and_set_151e6abb (libcis)

