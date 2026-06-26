// transferred+adapted from libc++ by tools/transfer.py (slug=containers_unord_unord_multiset_bucket_473ba83b).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <unordered_set>

// template <class Value, class Hash = hash<Value>, class Pred = equal_to<Value>,
//           class Alloc = allocator<Value>>
// class unordered_multiset

// size_type bucket(const key_type& __k) const;

#include <unordered_set>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"

namespace libcis_ns_containers_unord_unord_multiset_bucket_473ba83b { // libcis
int main(int, char**)
{
    {
        typedef std::unordered_multiset<int> C;
        typedef int P;
        P a[] =
        {
            P(1),
            P(2),
            P(3),
            P(4),
            P(1),
            P(2)
        };
        const C c(std::begin(a), std::end(a));
        std::size_t bc = c.bucket_count();
        assert(bc >= 7);
        for (std::size_t i = 0; i < 13; ++i)
            LIBCPP_ASSERT(c.bucket(i) == i % bc);
    }
#if TEST_STD_VER >= 11
    {
        typedef std::unordered_multiset<int, std::hash<int>,
                                      std::equal_to<int>, min_allocator<int>> C;
        typedef int P;
        P a[] =
        {
            P(1),
            P(2),
            P(3),
            P(4),
            P(1),
            P(2)
        };
        const C c(std::begin(a), std::end(a));
        std::size_t bc = c.bucket_count();
        assert(bc >= 7);
        for (std::size_t i = 0; i < 13; ++i)
            LIBCPP_ASSERT(c.bucket(i) == i % bc);
    }
#endif

    return 0;

    return 0;
}
} // libcis_ns_containers_unord_unord_multiset_bucket_473ba83b (libcis)

