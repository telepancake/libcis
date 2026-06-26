// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_obs_owns_lock_cdfb62b9).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: no-threads
// UNSUPPORTED: c++03, c++11

// <shared_mutex>

// template <class Mutex> class shared_lock;

// bool owns_lock() const noexcept;

#include <shared_mutex>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_obs_owns_lock_cdfb62b9 { // libcis
std::shared_timed_mutex m;

int main(int, char**)
{
    std::shared_lock<std::shared_timed_mutex> lk0;
    assert(lk0.owns_lock() == false);
    std::shared_lock<std::shared_timed_mutex> lk1(m);
    assert(lk1.owns_lock() == true);
    lk1.unlock();
    assert(lk1.owns_lock() == false);
    static_assert(noexcept(lk0.owns_lock()), "owns_lock must be noexcept");

  return 0;

    return 0;
}
} // libcis_ns_thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_obs_owns_lock_cdfb62b9 (libcis)

