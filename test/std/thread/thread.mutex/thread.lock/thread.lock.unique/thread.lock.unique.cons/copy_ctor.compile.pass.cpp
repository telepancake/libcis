// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_copy_ctor).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <mutex>

// template <class Mutex> class unique_lock;

// unique_lock(unique_lock const&) = delete;

#include <mutex>

#include "checking_mutex.h"

namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_copy_ctor { // libcis: isolate file-scope helpers
static_assert(!std::is_copy_constructible<std::lock_guard<checking_mutex> >::value, "");
} using namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_copy_ctor; // libcis

