// AST-transferred from libc++ by tools/transfer.py (slug=thread_futures_futures_task_futures_task_members_ctor_func).
// main -> test_thread_futures_futures_task_futures_task_members_ctor_func; file-scope helpers isolated in anon namespace.
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

// class packaged_task<R(ArgTypes...)>

// template <class F>
//     explicit packaged_task(F&& f);

#include <future>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_thread_futures_futures_task_futures_task_members_ctor_func { // libcis: isolate file-scope helpers
class A
{
    long data_;

public:
    static int n_moves;
    static int n_copies;

    explicit A(long i) : data_(i) {}
    A(A&& a) : data_(a.data_) {++n_moves; a.data_ = -1;}
    A(const A& a) : data_(a.data_) {++n_copies;}

    long operator()(long i, long j) const {return data_ + i + j;}
};
} using namespace libcis_ns_thread_futures_futures_task_futures_task_members_ctor_func; // libcis


int A::n_moves = 0;
int A::n_copies = 0;

namespace libcis_ns_thread_futures_futures_task_futures_task_members_ctor_func { // libcis: isolate file-scope helpers
int func(int i) { return i; }
} using namespace libcis_ns_thread_futures_futures_task_futures_task_members_ctor_func; // libcis


void test_thread_futures_futures_task_futures_task_members_ctor_func()
{
    {
        std::packaged_task<double(int, char)> p(A(5));
        assert(p.valid());
        std::future<double> f = p.get_future();
        p(3, 97);
        assert(f.get() == 105.0);
        assert(A::n_copies == 0);
        assert(A::n_moves > 0);
    }
    A::n_copies = 0;
    A::n_copies = 0;
    {
        A a(5);
        std::packaged_task<double(int, char)> p(a);
        assert(p.valid());
        std::future<double> f = p.get_future();
        p(3, 97);
        assert(f.get() == 105.0);
        assert(A::n_copies > 0);
        assert(A::n_moves > 0);
    }
    {
        std::packaged_task<int(int)> p(&func);
        assert(p.valid());
        std::future<int> f = p.get_future();
        p(4);
        assert(f.get() == 4);
    }
    {
        std::packaged_task<int(int)> p(func);
        assert(p.valid());
        std::future<int> f = p.get_future();
        p(4);
        assert(f.get() == 4);
    }

  return;
}
