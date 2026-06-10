// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_threads_thread_thread_this_yield).
// main -> test_thread_thread_threads_thread_thread_this_yield; file-scope helpers isolated in anon namespace.
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

// void this_thread::yield();

#include <thread>
#include <cassert>

#include "test_macros.h"

void test_thread_thread_threads_thread_thread_this_yield()
{
    std::this_thread::yield();

  return;
}
