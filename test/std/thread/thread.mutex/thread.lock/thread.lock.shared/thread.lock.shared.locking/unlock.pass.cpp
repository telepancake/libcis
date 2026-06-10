// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_locking_unlock).
// main -> test_thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_locking_unlock; file-scope helpers isolated in anon namespace.
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

// void unlock();

#include <cassert>
#include <cerrno>
#include <shared_mutex>
#include <system_error>

#include "test_macros.h"

namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_locking_unlock { // libcis: isolate file-scope helpers
bool unlock_called = false;

struct mutex
{
    void lock_shared() {}
    void unlock_shared() {unlock_called = true;}
};

mutex m;
} using namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_locking_unlock; // libcis


void test_thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_locking_unlock()
{
    std::shared_lock<mutex> lk(m);
    lk.unlock();
    assert(unlock_called == true);
    assert(lk.owns_lock() == false);
#ifndef TEST_HAS_NO_EXCEPTIONS
    try
    {
        lk.unlock();
        assert(false);
    }
    catch (std::system_error& e)
    {
        assert(e.code().value() == EPERM);
    }
#endif
    lk.release();
#ifndef TEST_HAS_NO_EXCEPTIONS
    try
    {
        lk.unlock();
        assert(false);
    }
    catch (std::system_error& e)
    {
        assert(e.code().value() == EPERM);
    }
#endif

  return;
}
