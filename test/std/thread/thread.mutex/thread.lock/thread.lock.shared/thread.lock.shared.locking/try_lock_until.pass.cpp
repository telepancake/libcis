// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_locking_try_lock_until).
// main -> test_thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_locking_try_lock_until; file-scope helpers isolated in anon namespace.
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

// template <class Clock, class Duration>
//   bool try_lock_until(const chrono::time_point<Clock, Duration>& abs_time);

#include <cassert>
#include <chrono>
#include <mutex>
#include <shared_mutex>
#include <system_error>

#include "test_macros.h"

namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_locking_try_lock_until { // libcis: isolate file-scope helpers
bool try_lock_until_called = false;

struct mutex
{
    template <class Clock, class Duration>
        bool try_lock_shared_until(const std::chrono::time_point<Clock, Duration>& abs_time)
    {
        typedef std::chrono::milliseconds ms;
        assert(Clock::now() - abs_time < ms(5));
        try_lock_until_called = !try_lock_until_called;
        return try_lock_until_called;
    }
    void unlock_shared() {}
};

mutex m;
} using namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_locking_try_lock_until; // libcis


void test_thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_locking_try_lock_until()
{
    typedef std::chrono::steady_clock Clock;
    std::shared_lock<mutex> lk(m, std::defer_lock);
    assert(lk.try_lock_until(Clock::now()) == true);
    assert(try_lock_until_called == true);
    assert(lk.owns_lock() == true);
#ifndef TEST_HAS_NO_EXCEPTIONS
    try
    {
        TEST_IGNORE_NODISCARD lk.try_lock_until(Clock::now());
        assert(false);
    }
    catch (std::system_error& e)
    {
        assert(e.code().value() == EDEADLK);
    }
#endif
    lk.unlock();
    assert(lk.try_lock_until(Clock::now()) == false);
    assert(try_lock_until_called == false);
    assert(lk.owns_lock() == false);
    lk.release();
#ifndef TEST_HAS_NO_EXCEPTIONS
    try
    {
        TEST_IGNORE_NODISCARD lk.try_lock_until(Clock::now());
        assert(false);
    }
    catch (std::system_error& e)
    {
        assert(e.code().value() == EPERM);
    }
#endif

  return;
}
