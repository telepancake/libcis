// transferred+adapted from libc++ by tools/transfer.py (slug=containers_container_adaptors_queue_queue_cons_alloc_ctor_container_alloc_89bdffd7).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <queue>

// template <class Alloc>
//   queue(const container_type& c, const Alloc& a);

#include <queue>
#include <cassert>
#include <cstddef>

#include "test_macros.h"
#include "test_allocator.h"

namespace libcis_ns_containers_container_adaptors_queue_queue_cons_alloc_ctor_container_alloc_89bdffd7 { // libcis
template <class C>
C
make(int n)
{
    C c;
    for (int i = 0; i < n; ++i)
        c.push_back(i);
    return c;
}

typedef std::deque<int, test_allocator<int> > C;

struct test
    : public std::queue<int, C>
{
    typedef std::queue<int, C> base;

    explicit test(const test_allocator<int>& a) : base(a) {}
    test(const container_type& container, const test_allocator<int>& a) : base(container, a) {}
#if TEST_STD_VER >= 11
    test(container_type&& container, const test_allocator<int>& a) : base(std::move(container), a) {}
    test(test&& q, const test_allocator<int>& a) : base(std::move(q), a) {}
#endif
    test_allocator<int> get_allocator() {return c.get_allocator();}
};

int main(int, char**)
{
    C d = make<C>(5);
    test q(d, test_allocator<int>(4));
    assert(q.get_allocator() == test_allocator<int>(4));
    assert(q.size() == 5);
    for (C::size_type i = 0; i < d.size(); ++i)
    {
        assert(q.front() == d[i]);
        q.pop();
    }

  return 0;

    return 0;
}
} // libcis_ns_containers_container_adaptors_queue_queue_cons_alloc_ctor_container_alloc_89bdffd7 (libcis)

