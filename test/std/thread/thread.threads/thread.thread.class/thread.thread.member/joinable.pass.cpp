// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_threads_thread_thread_class_thread_thread_member_joinable).
// main -> test_thread_thread_threads_thread_thread_class_thread_thread_member_joinable; file-scope helpers isolated in anon namespace.
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

// bool joinable() const;

#include <thread>
#include <new>
#include <cstdlib>
#include <cassert>

#include "make_test_thread.h"
#include "test_macros.h"

namespace libcis_ns_thread_thread_threads_thread_thread_class_thread_thread_member_joinable { // libcis: isolate file-scope helpers
class G
{
    int alive_;
public:
    static int n_alive;
    static bool op_run;

    G() : alive_(1) {++n_alive;}
    G(const G& g) : alive_(g.alive_) {++n_alive;}
    ~G() {alive_ = 0; --n_alive;}

    void operator()()
    {
        assert(alive_ == 1);
        assert(n_alive >= 1);
        op_run = true;
    }
};
} using namespace libcis_ns_thread_thread_threads_thread_thread_class_thread_thread_member_joinable; // libcis


int G::n_alive = 0;
bool G::op_run = false;

void test_thread_thread_threads_thread_thread_class_thread_thread_member_joinable()
{
    {
        G g;
        std::thread t0 = support::make_test_thread(g);
        assert(t0.joinable());
        t0.join();
        assert(!t0.joinable());
    }

  return;
}
