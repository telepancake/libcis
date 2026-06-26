// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_mutex_requirements_thread_sharedtimedmutex_requirements_thread_sharedtimedmutex_class_assign_5c759fe1).
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

// class shared_timed_mutex;

// shared_timed_mutex& operator=(const shared_timed_mutex&) = delete;

#include <shared_mutex>
#include <type_traits>

namespace libcis_ns_thread_thread_mutex_thread_mutex_requirements_thread_sharedtimedmutex_requirements_thread_sharedtimedmutex_class_assign_5c759fe1 { // libcis
static_assert(!std::is_copy_assignable<std::shared_timed_mutex>::value, "");
} // libcis_ns_thread_thread_mutex_thread_mutex_requirements_thread_sharedtimedmutex_requirements_thread_sharedtimedmutex_class_assign_5c759fe1 (libcis)

