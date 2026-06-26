// transferred+adapted from libc++ by tools/transfer.py (slug=thread_futures_futures_unique_future_default_b35c46bd).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: no-threads

// <future>

// class future<R>

// future();

#include <future>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_thread_futures_futures_unique_future_default_b35c46bd { // libcis
int main(int, char**)
{
    {
        std::future<int> f;
        assert(!f.valid());
    }
    {
        std::future<int&> f;
        assert(!f.valid());
    }
    {
        std::future<void> f;
        assert(!f.valid());
    }

  return 0;

    return 0;
}
} // libcis_ns_thread_futures_futures_unique_future_default_b35c46bd (libcis)

