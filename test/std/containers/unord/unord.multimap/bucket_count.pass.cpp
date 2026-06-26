// transferred+adapted from libc++ by tools/transfer.py (slug=containers_unord_unord_multimap_bucket_count_d31cf406).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <unordered_map>

// template <class Key, class T, class Hash = hash<Key>, class Pred = equal_to<Key>,
//           class Alloc = allocator<pair<const Key, T>>>
// class unordered_multimap

// size_type bucket_count() const;

#include <unordered_map>
#include <string>
#include <cassert>
#include <iterator>

#include "test_macros.h"

namespace libcis_ns_containers_unord_unord_multimap_bucket_count_d31cf406 { // libcis
int main(int, char**)
{
    {
        typedef std::unordered_multimap<int, std::string> C;
        const C c;
        LIBCPP_ASSERT(c.bucket_count() == 0);
    }
    {
        typedef std::unordered_multimap<int, std::string> C;
        typedef std::pair<int, std::string> P;
        P a[] =
        {
            P(10, "ten"),
            P(20, "twenty"),
            P(30, "thirty"),
            P(40, "forty"),
            P(50, "fifty"),
            P(60, "sixty"),
            P(70, "seventy"),
            P(80, "eighty"),
        };
        const C c(std::begin(a), std::end(a));
        assert(c.bucket_count() >= 8);
    }

  return 0;

    return 0;
}
} // libcis_ns_containers_unord_unord_multimap_bucket_count_d31cf406 (libcis)

