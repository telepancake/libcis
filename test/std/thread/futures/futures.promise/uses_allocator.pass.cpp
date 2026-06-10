// AST-transferred from libc++ by tools/transfer.py (slug=thread_futures_futures_promise_uses_allocator).
// main -> test_thread_futures_futures_promise_uses_allocator; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: no-threads

// <future>

// class promise<R>

// template <class R, class Alloc>
//   struct uses_allocator<promise<R>, Alloc>
//      : true_type { };

#include <future>
#include "test_macros.h"
#include "test_allocator.h"

void test_thread_futures_futures_promise_uses_allocator()
{
    static_assert((std::uses_allocator<std::promise<int>, test_allocator<int> >::value), "");
    static_assert((std::uses_allocator<std::promise<int&>, test_allocator<int> >::value), "");
    static_assert((std::uses_allocator<std::promise<void>, test_allocator<void> >::value), "");

  return;
}
