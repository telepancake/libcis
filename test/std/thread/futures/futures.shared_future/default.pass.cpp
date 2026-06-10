// AST-transferred from libc++ by tools/transfer.py (slug=thread_futures_futures_shared_future_default).
// main -> test_thread_futures_futures_shared_future_default; file-scope helpers isolated in anon namespace.
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

// class shared_future<R>

// shared_future();

#include <future>
#include <cassert>

#include "test_macros.h"

void test_thread_futures_futures_shared_future_default()
{
    {
        std::shared_future<int> f;
        assert(!f.valid());
    }
    {
        std::shared_future<int&> f;
        assert(!f.valid());
    }
    {
        std::shared_future<void> f;
        assert(!f.valid());
    }

  return;
}
