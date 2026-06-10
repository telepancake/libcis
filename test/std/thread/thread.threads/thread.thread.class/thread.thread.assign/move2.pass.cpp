// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_threads_thread_thread_class_thread_thread_assign_move2).
// main -> test_thread_thread_threads_thread_thread_class_thread_thread_assign_move2; file-scope helpers isolated in anon namespace.
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

// class thread

// thread& operator=(thread&& t);

#include <thread>
#include <cassert>
#include <cstdlib>
#include <exception>
#include <utility>

#include "make_test_thread.h"
#include "test_macros.h"

namespace libcis_ns_thread_thread_threads_thread_thread_class_thread_thread_assign_move2 { // libcis: isolate file-scope helpers
struct G
{
    void operator()() { }
};

void f1()
{
    std::_Exit(0);
}
} using namespace libcis_ns_thread_thread_threads_thread_thread_class_thread_thread_assign_move2; // libcis


void test_thread_thread_threads_thread_thread_class_thread_thread_assign_move2()
{
    std::set_terminate(f1);
    {
        G g;
        std::thread t0 = support::make_test_thread(g);
        std::thread t1;
        t0 = std::move(t1);
        assert(false);
    }

    return;
}
