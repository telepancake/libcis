// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_mutex_requirements_thread_shared_mutex_requirements_thread_shared_mutex_class_ctor_copy).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
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

// shared_mutex(const shared_mutex&) = delete;

#include <shared_mutex>
#include <type_traits>

namespace libcis_ns_thread_thread_mutex_thread_mutex_requirements_thread_shared_mutex_requirements_thread_shared_mutex_class_ctor_copy { // libcis: isolate file-scope helpers
static_assert(!std::is_copy_constructible<std::shared_mutex>::value, "");
} using namespace libcis_ns_thread_thread_mutex_thread_mutex_requirements_thread_shared_mutex_requirements_thread_shared_mutex_class_ctor_copy; // libcis

