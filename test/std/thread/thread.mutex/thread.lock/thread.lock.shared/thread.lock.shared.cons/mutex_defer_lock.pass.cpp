// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_cons_mutex_defer_lock).
// main -> test_thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_cons_mutex_defer_lock; file-scope helpers isolated in anon namespace.
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

// shared_lock(mutex_type& m, defer_lock_t);

#include <shared_mutex>
#include <cassert>
#include <mutex>
#include "nasty_containers.h"

#include "test_macros.h"

void test_thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_cons_mutex_defer_lock()
{
    {
    typedef std::shared_timed_mutex M;
    M m;
    std::unique_lock<M> lk(m, std::defer_lock);
    assert(lk.mutex() == std::addressof(m));
    assert(lk.owns_lock() == false);
    }
    {
    typedef nasty_mutex M;
    M m;
    std::unique_lock<M> lk(m, std::defer_lock);
    assert(lk.mutex() == std::addressof(m));
    assert(lk.owns_lock() == false);
    }

  return;
}
