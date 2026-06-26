// transferred+adapted from libc++ by tools/transfer.py (slug=containers_unord_unord_multiset_insert_rvalue_3a392a7a).
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

// iterator insert(value_type&& x);

#include <unordered_set>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"
#include "min_allocator.h"

namespace libcis_ns_containers_unord_unord_multiset_insert_rvalue_3a392a7a { // libcis
int main(int, char**)
{
    {
        typedef std::unordered_multiset<double> C;
        typedef C::iterator R;
        typedef double P;
        C c;
        R r = c.insert(P(3.5));
        assert(c.size() == 1);
        assert(*r == 3.5);

        r = c.insert(P(3.5));
        assert(c.size() == 2);
        assert(*r == 3.5);

        r = c.insert(P(4.5));
        assert(c.size() == 3);
        assert(*r == 4.5);

        r = c.insert(P(5.5));
        assert(c.size() == 4);
        assert(*r == 5.5);
    }
#if TEST_STD_VER >= 11
    {
        typedef std::unordered_multiset<MoveOnly> C;
        typedef C::iterator R;
        typedef MoveOnly P;
        C c;
        R r = c.insert(P(3));
        assert(c.size() == 1);
        assert(*r == 3);

        r = c.insert(P(3));
        assert(c.size() == 2);
        assert(*r == 3);

        r = c.insert(P(4));
        assert(c.size() == 3);
        assert(*r == 4);

        r = c.insert(P(5));
        assert(c.size() == 4);
        assert(*r == 5);
    }
    {
        typedef std::unordered_multiset<double, std::hash<double>,
                                std::equal_to<double>, min_allocator<double>> C;
        typedef C::iterator R;
        typedef double P;
        C c;
        R r = c.insert(P(3.5));
        assert(c.size() == 1);
        assert(*r == 3.5);

        r = c.insert(P(3.5));
        assert(c.size() == 2);
        assert(*r == 3.5);

        r = c.insert(P(4.5));
        assert(c.size() == 3);
        assert(*r == 4.5);

        r = c.insert(P(5.5));
        assert(c.size() == 4);
        assert(*r == 5.5);
    }
    {
        typedef std::unordered_multiset<MoveOnly, std::hash<MoveOnly>,
                            std::equal_to<MoveOnly>, min_allocator<MoveOnly>> C;
        typedef C::iterator R;
        typedef MoveOnly P;
        C c;
        R r = c.insert(P(3));
        assert(c.size() == 1);
        assert(*r == 3);

        r = c.insert(P(3));
        assert(c.size() == 2);
        assert(*r == 3);

        r = c.insert(P(4));
        assert(c.size() == 3);
        assert(*r == 4);

        r = c.insert(P(5));
        assert(c.size() == 4);
        assert(*r == 5);
    }
#endif // TEST_STD_VER >= 11

  return 0;

    return 0;
}
} // libcis_ns_containers_unord_unord_multiset_insert_rvalue_3a392a7a (libcis)

