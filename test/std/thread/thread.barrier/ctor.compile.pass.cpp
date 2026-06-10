// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_barrier_ctor).
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

// <barrier>

// explicit barrier(ptrdiff_t __count, _CompletionF __completion = _CompletionF());

// Make sure that the ctor of barrier is explicit.

#include <barrier>

#include "test_convertible.h"

namespace libcis_ns_thread_thread_barrier_ctor { // libcis: isolate file-scope helpers
static_assert(!test_convertible<std::barrier<>, std::ptrdiff_t>(), "This constructor must be explicit");
} using namespace libcis_ns_thread_thread_barrier_ctor; // libcis

