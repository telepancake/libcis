// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_condition_thread_condition_condvar_wait).
// main -> test_thread_thread_condition_thread_condition_condvar_wait; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: no-threads

// <condition_variable>

// class condition_variable;

// void wait(unique_lock<mutex>& lock);

#include <condition_variable>
#include <mutex>
#include <thread>
#include <cassert>

#include "make_test_thread.h"
#include "test_macros.h"

namespace libcis_ns_thread_thread_condition_thread_condition_condvar_wait { // libcis: isolate file-scope helpers
std::condition_variable cv;
std::mutex mut;

int test1 = 0;
int test2 = 0;

void f()
{
    std::unique_lock<std::mutex> lk(mut);
    assert(test2 == 0);
    test1 = 1;
    cv.notify_one();
    while (test2 == 0)
        cv.wait(lk);
    assert(test2 != 0);
}
} using namespace libcis_ns_thread_thread_condition_thread_condition_condvar_wait; // libcis


void test_thread_thread_condition_thread_condition_condvar_wait()
{
    std::unique_lock<std::mutex> lk(mut);
    std::thread t = support::make_test_thread(f);
    assert(test1 == 0);
    while (test1 == 0)
        cv.wait(lk);
    assert(test1 != 0);
    test2 = 1;
    lk.unlock();
    cv.notify_one();
    t.join();

  return;
}
