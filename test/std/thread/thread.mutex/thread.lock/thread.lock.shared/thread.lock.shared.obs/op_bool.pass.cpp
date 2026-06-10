// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_obs_op_bool).
// main -> test_thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_obs_op_bool; file-scope helpers isolated in anon namespace.
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

// explicit operator bool() const noexcept;

#include <shared_mutex>
#include <cassert>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_obs_op_bool { // libcis: isolate file-scope helpers
struct M {
    void lock_shared() {}
    void unlock_shared() {}
};
} using namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_obs_op_bool; // libcis


void test_thread_thread_mutex_thread_lock_thread_lock_shared_thread_lock_shared_obs_op_bool()
{
    static_assert(std::is_constructible<bool, std::shared_lock<M>>::value, "");
    static_assert(!std::is_convertible<std::shared_lock<M>, bool>::value, "");

    M m;
    std::shared_lock<M> lk0;
    assert(static_cast<bool>(lk0) == false);
    std::shared_lock<M> lk1(m);
    assert(static_cast<bool>(lk1) == true);
    lk1.unlock();
    assert(static_cast<bool>(lk1) == false);
    ASSERT_NOEXCEPT(static_cast<bool>(lk0));

    return;
}
