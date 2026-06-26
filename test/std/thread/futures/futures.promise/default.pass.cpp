// transferred+adapted from libc++ by tools/transfer.py (slug=thread_futures_futures_promise_default_c9dacd56).
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

// promise();

#include <future>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_thread_futures_futures_promise_default_c9dacd56 { // libcis
int main(int, char**)
{
    {
        std::promise<int> p;
        std::future<int> f = p.get_future();
        assert(f.valid());
    }
    {
        std::promise<int&> p;
        std::future<int&> f = p.get_future();
        assert(f.valid());
    }
    {
        std::promise<void> p;
        std::future<void> f = p.get_future();
        assert(f.valid());
    }

  return 0;

    return 0;
}
} // libcis_ns_thread_futures_futures_promise_default_c9dacd56 (libcis)

