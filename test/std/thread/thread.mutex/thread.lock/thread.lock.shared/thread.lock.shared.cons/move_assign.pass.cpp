// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_cons_move_assign).
// main -> test_thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_cons_move_assign; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: no-threads
// UNSUPPORTED: c++03, c++11

// <shared_mutex>

// template <class Mutex> class shared_lock;

// shared_lock& operator=(shared_lock&& u) noexcept;

#include <cassert>
#include <shared_mutex>
#include <type_traits>

#include "nasty_containers.h"
#include "test_macros.h"

void test_thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_cons_move_assign() {
  {
    typedef std::shared_timed_mutex M;
    M m0;
    M m1;
    std::shared_lock<M> lk0(m0);
    std::shared_lock<M> lk1(m1);
    lk1 = std::move(lk0);
    assert(lk1.mutex() == std::addressof(m0));
    assert(lk1.owns_lock() == true);
    assert(lk0.mutex() == nullptr);
    assert(lk0.owns_lock() == false);

    static_assert(std::is_nothrow_move_assignable<std::shared_lock<M> >::value, "");
  }
  {
    typedef nasty_mutex M;
    M m0;
    M m1;
    std::shared_lock<M> lk0(m0);
    std::shared_lock<M> lk1(m1);
    lk1 = std::move(lk0);
    assert(lk1.mutex() == std::addressof(m0));
    assert(lk1.owns_lock() == true);
    assert(lk0.mutex() == nullptr);
    assert(lk0.owns_lock() == false);

    static_assert(std::is_nothrow_move_assignable<std::shared_lock<M> >::value, "");
  }
  {
    // Test self move-assignment (LWG4172)
    typedef std::shared_timed_mutex M;
    M m0;
    std::shared_lock<M> lk0(m0);
    lk0 = std::move(lk0);
    assert(lk0.mutex() == std::addressof(m0));
    assert(lk0.owns_lock() == true);
  }

  return;
}
