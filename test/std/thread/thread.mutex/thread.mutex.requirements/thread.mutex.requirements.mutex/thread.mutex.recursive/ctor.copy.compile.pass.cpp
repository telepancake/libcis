// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_mutex_requirements_thread_mutex_requirements_mutex_thread_mutex_recursive_ctor_copy_497206be).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: no-threads

// <mutex>

// class recursive_mutex;

// recursive_mutex(const recursive_mutex&) = delete;

#include <mutex>
#include <type_traits>

namespace libcis_ns_thread_thread_mutex_thread_mutex_requirements_thread_mutex_requirements_mutex_thread_mutex_recursive_ctor_copy_497206be { // libcis
static_assert(!std::is_copy_constructible<std::recursive_mutex>::value, "");
} // libcis_ns_thread_thread_mutex_thread_mutex_requirements_thread_mutex_requirements_mutex_thread_mutex_recursive_ctor_copy_497206be (libcis)

