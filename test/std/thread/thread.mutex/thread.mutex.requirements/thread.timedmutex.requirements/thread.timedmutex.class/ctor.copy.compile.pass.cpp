// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_mutex_requirements_thread_timedmutex_requirements_thread_timedmutex_class_ctor_copy_fa9bb3f7).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: no-threads

// <mutex>

// class timed_mutex;

// timed_mutex(const timed_mutex&) = delete;

#include <mutex>
#include <type_traits>

namespace libcis_ns_thread_thread_mutex_thread_mutex_requirements_thread_timedmutex_requirements_thread_timedmutex_class_ctor_copy_fa9bb3f7 { // libcis
static_assert(!std::is_copy_constructible<std::timed_mutex>::value, "");
} // libcis_ns_thread_thread_mutex_thread_mutex_requirements_thread_timedmutex_requirements_thread_timedmutex_class_ctor_copy_fa9bb3f7 (libcis)

