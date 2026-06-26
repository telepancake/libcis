// transferred+adapted from libc++ by tools/transfer.py (slug=thread_futures_futures_promise_set_value_at_thread_exit_void_02e45adb).
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

namespace libcis_ns_thread_futures_futures_promise_set_value_at_thread_exit_void_02e45adb { // libcis
int i = 0;

void func(std::promise<void> p)
{
    p.set_value_at_thread_exit();
    i = 1;
}

int main(int, char**)
{
    {
        std::promise<void> p;
        std::future<void> f = p.get_future();
        support::make_test_thread(func, std::move(p)).detach();
        f.get();
        assert(i == 1);
    }

  return 0;

    return 0;
}
} // libcis_ns_thread_futures_futures_promise_set_value_at_thread_exit_void_02e45adb (libcis)

