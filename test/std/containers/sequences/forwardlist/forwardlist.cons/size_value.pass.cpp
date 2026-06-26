// transferred+adapted from libc++ by tools/transfer.py (slug=containers_sequences_forwardlist_forwardlist_cons_size_value_595f9587).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <forward_list>

// forward_list(size_type n, const value_type& v);

#include <forward_list>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"

namespace libcis_ns_containers_sequences_forwardlist_forwardlist_cons_size_value_595f9587 { // libcis
int main(int, char**)
{
    {
        typedef int T;
        typedef std::forward_list<T> C;
        T v(6);
        unsigned N = 10;
        C c(N, v);
        unsigned n = 0;
        for (C::const_iterator i = c.begin(), e = c.end(); i != e; ++i, ++n)
            assert(*i == v);
        assert(n == N);
    }
#if TEST_STD_VER >= 11
    {
        typedef int T;
        typedef std::forward_list<T, min_allocator<T>> C;
        T v(6);
        unsigned N = 10;
        C c(N, v);
        unsigned n = 0;
        for (C::const_iterator i = c.begin(), e = c.end(); i != e; ++i, ++n)
            assert(*i == v);
        assert(n == N);
    }
#endif

  return 0;

    return 0;
}
} // libcis_ns_containers_sequences_forwardlist_forwardlist_cons_size_value_595f9587 (libcis)

