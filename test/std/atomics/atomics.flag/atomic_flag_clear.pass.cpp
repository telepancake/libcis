// transferred+adapted from libc++ by tools/transfer.py (slug=atomics_atomics_flag_atomic_flag_clear_e59dcda3).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <atomic>

// struct atomic_flag

// void atomic_flag_clear(volatile atomic_flag*);
// void atomic_flag_clear(atomic_flag*);

#include <atomic>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_atomics_atomics_flag_atomic_flag_clear_e59dcda3 { // libcis
int main(int, char**)
{
    {
        std::atomic_flag f;
        f.clear();
        f.test_and_set();
        std::atomic_flag_clear(&f);
        assert(f.test_and_set() == 0);
    }
    {
        volatile std::atomic_flag f;
        f.clear();
        f.test_and_set();
        std::atomic_flag_clear(&f);
        assert(f.test_and_set() == 0);
    }

  return 0;

    return 0;
}
} // libcis_ns_atomics_atomics_flag_atomic_flag_clear_e59dcda3 (libcis)

