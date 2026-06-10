// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_jthread_type).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: no-threads
// UNSUPPORTED: c++03, c++11, c++14, c++17

//  using id = thread::id;
//  using native_handle_type = thread::native_handle_type;

#include <thread>
#include <type_traits>

namespace libcis_ns_thread_thread_jthread_type { // libcis: isolate file-scope helpers
static_assert(std::is_same_v<std::jthread::id, std::thread::id>);
static_assert(std::is_same_v<std::jthread::native_handle_type, std::thread::native_handle_type>);
} using namespace libcis_ns_thread_thread_jthread_type; // libcis

