// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_lock_thread_lock_shared_types).
// main -> test_thread_thread_mutex_thread_lock_thread_lock_shared_types; file-scope helpers isolated in anon namespace.
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

// template <class Mutex>
// class shared_lock
// {
// public:
//     typedef Mutex mutex_type;
//     ...
// };

#include <shared_mutex>
#include <mutex>
#include <type_traits>

#include "test_macros.h"

void test_thread_thread_mutex_thread_lock_thread_lock_shared_types()
{
    static_assert((std::is_same<std::shared_lock<std::mutex>::mutex_type,
                   std::mutex>::value), "");

  return;
}
