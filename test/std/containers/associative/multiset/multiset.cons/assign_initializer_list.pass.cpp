// transferred+adapted from libc++ by tools/transfer.py (slug=containers_associative_multiset_multiset_cons_assign_initializer_list_47f68207).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <set>

// class multiset

// multiset& operator=(initializer_list<value_type> il);

#include <set>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"

namespace libcis_ns_containers_associative_multiset_multiset_cons_assign_initializer_list_47f68207 { // libcis
int main(int, char**)
{
    {
    typedef std::multiset<int> C;
    typedef C::value_type V;
    C m = {10, 8};
    m = {1, 2, 3, 4, 5, 6};
    assert(m.size() == 6);
    assert(std::distance(m.begin(), m.end()) == 6);
    C::const_iterator i = m.cbegin();
    assert(*i == V(1));
    assert(*++i == V(2));
    assert(*++i == V(3));
    assert(*++i == V(4));
    assert(*++i == V(5));
    assert(*++i == V(6));
    }
    {
    typedef std::multiset<int, std::less<int>, min_allocator<int>> C;
    typedef C::value_type V;
    C m = {10, 8};
    m = {1, 2, 3, 4, 5, 6};
    assert(m.size() == 6);
    assert(std::distance(m.begin(), m.end()) == 6);
    C::const_iterator i = m.cbegin();
    assert(*i == V(1));
    assert(*++i == V(2));
    assert(*++i == V(3));
    assert(*++i == V(4));
    assert(*++i == V(5));
    assert(*++i == V(6));
    }

  return 0;

    return 0;
}
} // libcis_ns_containers_associative_multiset_multiset_cons_assign_initializer_list_47f68207 (libcis)

