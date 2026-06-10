// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_threads_thread_thread_class_thread_thread_id_copy).
// main -> test_thread_thread_threads_thread_thread_class_thread_thread_id_copy; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: no-threads

// <thread>

// class thread::id

// id(const id&) = default;

#include <thread>
#include <cassert>

#include "test_macros.h"

void test_thread_thread_threads_thread_thread_class_thread_thread_id_copy()
{
    std::thread::id id0;
    std::thread::id id1 = id0;
    assert(id1 == id0);

  return;
}
