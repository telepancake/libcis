// AST-transferred from libc++ by tools/transfer.py (slug=thread_futures_futures_task_futures_task_members_reset).
// main -> test_thread_futures_futures_task_futures_task_members_reset; file-scope helpers isolated in anon namespace.
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

// void reset();

#include <future>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_thread_futures_futures_task_futures_task_members_reset { // libcis: isolate file-scope helpers
class A
{
    long data_;

public:
    explicit A(long i) : data_(i) {}

    long operator()(long i, long j) const
    {
        return data_ + i + j;
    }
};
} using namespace libcis_ns_thread_futures_futures_task_futures_task_members_reset; // libcis


void test_thread_futures_futures_task_futures_task_members_reset()
{
    {
        std::packaged_task<double(int, char)> p(A(5));
        std::future<double> f = p.get_future();
        p(3, 97);
        assert(f.get() == 105.0);
        p.reset();
        p(4, 97);
        f = p.get_future();
        assert(f.get() == 106.0);
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    {
        std::packaged_task<double(int, char)> p;
        try
        {
            p.reset();
            assert(false);
        }
        catch (const std::future_error& e)
        {
            assert(e.code() == make_error_code(std::future_errc::no_state));
        }
    }
#endif

  return;
}
