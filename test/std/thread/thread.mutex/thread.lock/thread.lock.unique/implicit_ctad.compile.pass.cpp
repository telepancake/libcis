// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_lock_thread_lock_unique_implicit_ctad).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <mutex>

// unique_lock

// Make sure that the implicitly-generated CTAD works.

#include <mutex>

#include "checking_mutex.h"

namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_unique_implicit_ctad { // libcis: isolate file-scope helpers
checking_mutex mux;
static_assert(std::is_same_v<std::unique_lock<checking_mutex>, decltype(std::unique_lock{mux})>);
} using namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_unique_implicit_ctad; // libcis

