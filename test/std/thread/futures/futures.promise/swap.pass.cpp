// transferred+adapted from libc++ by tools/transfer.py (slug=thread_futures_futures_promise_swap_177d00e8).
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

// void swap(promise& other);

// template <class R> void swap(promise<R>& x, promise<R>& y);

#include <future>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"

namespace libcis_ns_thread_futures_futures_promise_swap_177d00e8 { // libcis
int main(int, char**)
{
    test_allocator_statistics alloc_stats;
    assert(alloc_stats.alloc_count == 0);
    {
        std::promise<int> p0(std::allocator_arg, test_allocator<int>(&alloc_stats));
        std::promise<int> p(std::allocator_arg, test_allocator<int>(&alloc_stats));
        assert(alloc_stats.alloc_count == 2);
        p.swap(p0);
        assert(alloc_stats.alloc_count == 2);
        std::future<int> f = p.get_future();
        assert(alloc_stats.alloc_count == 2);
        assert(f.valid());
        f = p0.get_future();
        assert(f.valid());
        assert(alloc_stats.alloc_count == 2);
    }
    assert(alloc_stats.alloc_count == 0);
    {
        std::promise<int> p0(std::allocator_arg, test_allocator<int>(&alloc_stats));
        std::promise<int> p(std::allocator_arg, test_allocator<int>(&alloc_stats));
        assert(alloc_stats.alloc_count == 2);
        swap(p, p0);
        assert(alloc_stats.alloc_count == 2);
        std::future<int> f = p.get_future();
        assert(alloc_stats.alloc_count == 2);
        assert(f.valid());
        f = p0.get_future();
        assert(f.valid());
        assert(alloc_stats.alloc_count == 2);
    }
    assert(alloc_stats.alloc_count == 0);
    {
        std::promise<int> p0(std::allocator_arg, test_allocator<int>(&alloc_stats));
        std::promise<int> p;
        assert(alloc_stats.alloc_count == 1);
        p.swap(p0);
        assert(alloc_stats.alloc_count == 1);
        std::future<int> f = p.get_future();
        assert(alloc_stats.alloc_count == 1);
        assert(f.valid());
        f = p0.get_future();
        assert(f.valid());
        assert(alloc_stats.alloc_count == 1);
    }
    assert(alloc_stats.alloc_count == 0);
    {
        std::promise<int> p0(std::allocator_arg, test_allocator<int>(&alloc_stats));
        std::promise<int> p;
        assert(alloc_stats.alloc_count == 1);
        swap(p, p0);
        assert(alloc_stats.alloc_count == 1);
        std::future<int> f = p.get_future();
        assert(alloc_stats.alloc_count == 1);
        assert(f.valid());
        f = p0.get_future();
        assert(f.valid());
        assert(alloc_stats.alloc_count == 1);
    }
    assert(alloc_stats.alloc_count == 0);

  return 0;

    return 0;
}
} // libcis_ns_thread_futures_futures_promise_swap_177d00e8 (libcis)

