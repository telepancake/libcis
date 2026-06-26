// transferred+adapted from libc++ by tools/transfer.py (slug=containers_sequences_forwardlist_forwardlist_cons_range_alloc_502108f5).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <forward_list>

// template <class InputIterator>
//     forward_list(InputIterator first, InputIterator last,
//                  const allocator_type& a);

#include <forward_list>
#include <cassert>
#include <iterator>

#include "test_macros.h"
#include "test_allocator.h"
#include "test_iterators.h"
#include "min_allocator.h"

namespace libcis_ns_containers_sequences_forwardlist_forwardlist_cons_range_alloc_502108f5 { // libcis
int main(int, char**)
{
    {
        typedef int T;
        typedef test_allocator<T> A;
        typedef std::forward_list<T, A> C;
        typedef cpp17_input_iterator<const T*> I;
        const T t[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        C c(I(std::begin(t)), I(std::end(t)), A(13));
        int n = 0;
        for (C::const_iterator i = c.begin(), e = c.end(); i != e; ++i, ++n)
            assert(*i == n);
        assert(n == std::end(t) - std::begin(t));
        assert(c.get_allocator() == A(13));
    }
#if TEST_STD_VER >= 11
    {
        typedef int T;
        typedef min_allocator<T> A;
        typedef std::forward_list<T, A> C;
        typedef cpp17_input_iterator<const T*> I;
        const T t[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        C c(I(std::begin(t)), I(std::end(t)), A());
        int n = 0;
        for (C::const_iterator i = c.begin(), e = c.end(); i != e; ++i, ++n)
            assert(*i == n);
        assert(n == std::end(t) - std::begin(t));
        assert(c.get_allocator() == A());
    }
#endif

  return 0;

    return 0;
}
} // libcis_ns_containers_sequences_forwardlist_forwardlist_cons_range_alloc_502108f5 (libcis)

