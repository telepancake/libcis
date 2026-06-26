// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_mutex_requirements_thread_shared_mutex_requirements_thread_shared_mutex_class_assign_06d6c979).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: no-threads
// UNSUPPORTED: c++03, c++11, c++14

// <shared_mutex>

// class shared_mutex;

// shared_mutex& operator=(const shared_mutex&) = delete;

#include <shared_mutex>
#include <type_traits>

namespace libcis_ns_thread_thread_mutex_thread_mutex_requirements_thread_shared_mutex_requirements_thread_shared_mutex_class_assign_06d6c979 { // libcis
static_assert(!std::is_copy_assignable<std::shared_mutex>::value, "");
} // libcis_ns_thread_thread_mutex_thread_mutex_requirements_thread_shared_mutex_requirements_thread_shared_mutex_class_assign_06d6c979 (libcis)

