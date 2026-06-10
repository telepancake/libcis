// AST-transferred from libc++ by tools/transfer.py (slug=thread_futures_futures_promise_set_value_at_thread_exit_void).
// main -> test_thread_futures_futures_promise_set_value_at_thread_exit_void; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: no-threads
// UNSUPPORTED: c++03

// <future>

// class promise<R>

// void promise<void>::set_value_at_thread_exit();

#include <future>
#include <memory>
#include <cassert>

#include "make_test_thread.h"
#include "test_macros.h"

namespace libcis_ns_thread_futures_futures_promise_set_value_at_thread_exit_void { // libcis: isolate file-scope helpers
int i = 0;

void func(std::promise<void> p)
{
    p.set_value_at_thread_exit();
    i = 1;
}
} using namespace libcis_ns_thread_futures_futures_promise_set_value_at_thread_exit_void; // libcis


void test_thread_futures_futures_promise_set_value_at_thread_exit_void()
{
    {
        std::promise<void> p;
        std::future<void> f = p.get_future();
        support::make_test_thread(func, std::move(p)).detach();
        f.get();
        assert(i == 1);
    }

  return;
}
