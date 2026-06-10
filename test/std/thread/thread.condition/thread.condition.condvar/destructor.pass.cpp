// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_condition_thread_condition_condvar_destructor).
// main -> test_thread_thread_condition_thread_condition_condvar_destructor; file-scope helpers isolated in anon namespace.
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

// ~condition_variable();

#include <condition_variable>
#include <mutex>
#include <thread>
#include <cassert>

#include "make_test_thread.h"
#include "test_macros.h"

namespace libcis_ns_thread_thread_condition_thread_condition_condvar_destructor { // libcis: isolate file-scope helpers
std::condition_variable* cv;
std::mutex m;
typedef std::unique_lock<std::mutex> Lock;

bool f_ready = false;
bool g_ready = false;

void f()
{
    Lock lk(m);
    f_ready = true;
    cv->notify_one();
    delete cv;
}

void g()
{
    Lock lk(m);
    g_ready = true;
    cv->notify_one();
    while (!f_ready)
        cv->wait(lk);
}
} using namespace libcis_ns_thread_thread_condition_thread_condition_condvar_destructor; // libcis


void test_thread_thread_condition_thread_condition_condvar_destructor()
{
    cv = new std::condition_variable;
    std::thread th2 = support::make_test_thread(g);
    Lock lk(m);
    while (!g_ready)
        cv->wait(lk);
    lk.unlock();
    std::thread th1 = support::make_test_thread(f);
    th1.join();
    th2.join();

  return;
}
