// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_mod_member_swap).
// main -> test_thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_mod_member_swap; file-scope helpers isolated in anon namespace.
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

// void swap(shared_lock& u) noexcept;

#include <shared_mutex>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_mod_member_swap { // libcis: isolate file-scope helpers
struct mutex
{
    void lock_shared() {}
    void unlock_shared() {}
};

mutex m;
} using namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_mod_member_swap; // libcis


void test_thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_mod_member_swap()
{
    std::shared_lock<mutex> lk1(m);
    std::shared_lock<mutex> lk2;
    lk1.swap(lk2);
    assert(lk1.mutex() == nullptr);
    assert(lk1.owns_lock() == false);
    assert(lk2.mutex() == &m);
    assert(lk2.owns_lock() == true);
    static_assert(noexcept(lk1.swap(lk2)), "member swap must be noexcept");

  return;
}
