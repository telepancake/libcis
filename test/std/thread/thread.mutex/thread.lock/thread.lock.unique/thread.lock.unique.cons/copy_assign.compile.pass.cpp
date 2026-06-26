// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_copy_assign_302ae692).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <mutex>

// template <class Mutex> class unique_lock;

// unique_lock& operator=(unique_lock const&) = delete;

#include <mutex>

#include "checking_mutex.h"

namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_copy_assign_302ae692 { // libcis
static_assert(!std::is_copy_assignable<std::lock_guard<checking_mutex> >::value, "");
} // libcis_ns_thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_copy_assign_302ae692 (libcis)

