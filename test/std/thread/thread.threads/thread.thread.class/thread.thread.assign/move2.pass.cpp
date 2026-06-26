// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_threads_thread_thread_class_thread_thread_assign_move2_5fb0b49c).
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

namespace libcis_ns_thread_thread_threads_thread_thread_class_thread_thread_assign_move2_5fb0b49c { // libcis
struct G
{
    void operator()() { }
};

void f1()
{
    std::_Exit(0);
}

int main(int, char**)
{
    std::set_terminate(f1);
    {
        G g;
        std::thread t0 = support::make_test_thread(g);
        std::thread t1;
        t0 = std::move(t1);
        assert(false);
    }

    return 0;

    return 0;
}
} // libcis_ns_thread_thread_threads_thread_thread_class_thread_thread_assign_move2_5fb0b49c (libcis)

