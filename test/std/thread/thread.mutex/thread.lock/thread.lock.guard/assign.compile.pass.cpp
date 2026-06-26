// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_lock_thread_lock_guard_assign_4cc13032).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <mutex>

// template <class Mutex> class lock_guard;

// lock_guard& operator=(lock_guard const&) = delete;

#include <mutex>

#include "types.h"

namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_guard_assign_4cc13032 { // libcis
static_assert(!std::is_copy_assignable<std::lock_guard<MyMutex> >::value, "");
} // libcis_ns_thread_thread_mutex_thread_lock_thread_lock_guard_assign_4cc13032 (libcis)

