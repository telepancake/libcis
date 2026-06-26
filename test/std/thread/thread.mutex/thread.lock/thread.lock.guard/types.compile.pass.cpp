// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_lock_thread_lock_guard_types_d7df90be).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <mutex>

// template <class Mutex>
// class lock_guard
// {
// public:
//     typedef Mutex mutex_type;
//     ...
// };

#include <mutex>
#include <type_traits>

#include "types.h"

namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_guard_types_d7df90be { // libcis
static_assert(std::is_same<std::lock_guard<MyMutex>::mutex_type, MyMutex>::value, "");
} // libcis_ns_thread_thread_mutex_thread_lock_thread_lock_guard_types_d7df90be (libcis)

