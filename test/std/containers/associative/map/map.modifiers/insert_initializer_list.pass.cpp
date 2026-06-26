// transferred+adapted from libc++ by tools/transfer.py (slug=containers_associative_map_map_modifiers_insert_initializer_list_273d4ddc).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <map>

// class map

// void insert(initializer_list<value_type> il);

#include <map>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"

namespace libcis_ns_containers_associative_map_map_modifiers_insert_initializer_list_273d4ddc { // libcis
int main(int, char**)
{
    {
    typedef std::pair<const int, double> V;
    std::map<int, double> m =
                            {
                                {1, 1},
                                {1, 1.5},
                                {1, 2},
                                {3, 1},
                                {3, 1.5},
                                {3, 2}
                            };
    m.insert({
                 {2, 1},
                 {2, 1.5},
                 {2, 2},
             });
    assert(m.size() == 3);
    assert(std::distance(m.begin(), m.end()) == 3);
    assert(*m.begin() == V(1, 1));
    assert(*std::next(m.begin()) == V(2, 1));
    assert(*std::next(m.begin(), 2) == V(3, 1));
    }
    {
    typedef std::pair<const int, double> V;
    std::map<int, double, std::less<int>, min_allocator<V>> m =
                            {
                                {1, 1},
                                {1, 1.5},
                                {1, 2},
                                {3, 1},
                                {3, 1.5},
                                {3, 2}
                            };
    m.insert({
                 {2, 1},
                 {2, 1.5},
                 {2, 2},
             });
    assert(m.size() == 3);
    assert(std::distance(m.begin(), m.end()) == 3);
    assert(*m.begin() == V(1, 1));
    assert(*std::next(m.begin()) == V(2, 1));
    assert(*std::next(m.begin(), 2) == V(3, 1));
    }

  return 0;

    return 0;
}
} // libcis_ns_containers_associative_map_map_modifiers_insert_initializer_list_273d4ddc (libcis)

