// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_mod_release).
// main -> test_thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_mod_release; file-scope helpers isolated in anon namespace.
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

// mutex_type* release() noexcept;

#include <shared_mutex>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_mod_release { // libcis: isolate file-scope helpers
struct mutex
{
    static int lock_count;
    static int unlock_count;
    void lock_shared() {++lock_count;}
    void unlock_shared() {++unlock_count;}
};
} using namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_mod_release; // libcis


int mutex::lock_count = 0;
int mutex::unlock_count = 0;

namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_mod_release { // libcis: isolate file-scope helpers
mutex m;
} using namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_mod_release; // libcis


void test_thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_mod_release()
{
    std::shared_lock<mutex> lk(m);
    assert(lk.mutex() == &m);
    assert(lk.owns_lock() == true);
    assert(mutex::lock_count == 1);
    assert(mutex::unlock_count == 0);
    assert(lk.release() == &m);
    assert(lk.mutex() == nullptr);
    assert(lk.owns_lock() == false);
    assert(mutex::lock_count == 1);
    assert(mutex::unlock_count == 0);
    static_assert(noexcept(lk.release()), "release must be noexcept");

  return;
}
