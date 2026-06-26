// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_mutex_requirements_thread_timedmutex_requirements_thread_timedmutex_recursive_ctor_copy_84b55d95).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: no-threads

// <mutex>

// class recursive_timed_mutex;

// recursive_timed_mutex(const recursive_timed_mutex&) = delete;

#include <mutex>
#include <type_traits>

namespace libcis_ns_thread_thread_mutex_thread_mutex_requirements_thread_timedmutex_requirements_thread_timedmutex_recursive_ctor_copy_84b55d95 { // libcis
static_assert(!std::is_copy_constructible<std::recursive_timed_mutex>::value, "");
} // libcis_ns_thread_thread_mutex_thread_mutex_requirements_thread_timedmutex_requirements_thread_timedmutex_recursive_ctor_copy_84b55d95 (libcis)

