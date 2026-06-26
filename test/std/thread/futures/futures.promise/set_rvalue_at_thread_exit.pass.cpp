// transferred+adapted from libc++ by tools/transfer.py (slug=thread_futures_futures_promise_set_rvalue_at_thread_exit_15f23685).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: no-threads, c++03

// <future>

// class promise<R>

// void promise::set_value_at_thread_exit(R&& r);

#include <future>
#include <memory>
#include <cassert>

#include "make_test_thread.h"
#include "test_macros.h"

namespace libcis_ns_thread_futures_futures_promise_set_rvalue_at_thread_exit_15f23685 { // libcis
void func(std::promise<std::unique_ptr<int>> p)
{
    p.set_value_at_thread_exit(std::unique_ptr<int>(new int(5)));
}

int main(int, char**)
{
    {
        std::promise<std::unique_ptr<int>> p;
        std::future<std::unique_ptr<int>> f = p.get_future();
        support::make_test_thread(func, std::move(p)).detach();
        assert(*f.get() == 5);
    }

  return 0;

    return 0;
}
} // libcis_ns_thread_futures_futures_promise_set_rvalue_at_thread_exit_15f23685 (libcis)

