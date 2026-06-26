// transferred+adapted from libc++ by tools/transfer.py (slug=containers_unord_unord_multimap_unord_multimap_modifiers_insert_node_type_hint_2ee1cdab).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <unordered_map>

// class unordered_multimap

// iterator insert(const_iterator hint, node_type&&);

#include <unordered_map>
#include "test_macros.h"
#include "min_allocator.h"

namespace libcis_ns_containers_unord_unord_multimap_unord_multimap_modifiers_insert_node_type_hint_2ee1cdab { // libcis
template <class Container>
typename Container::node_type
node_factory(typename Container::key_type const& key,
             typename Container::mapped_type const& mapped)
{
    static Container c;
    auto it = c.insert({key, mapped});
    return c.extract(it);
}

template <class Container>
void test(Container& c)
{
    auto* nf = &node_factory<Container>;

    for (int i = 0; i != 10; ++i)
    {
        typename Container::node_type node = nf(i, i + 1);
        assert(!node.empty());
        std::size_t prev = c.size();
        auto it = c.insert(c.end(), std::move(node));
        assert(node.empty());
        assert(prev + 1 == c.size());
        assert(it->first == i);
        assert(it->second == i + 1);
    }

    assert(c.size() == 10);

    for (int i = 0; i != 10; ++i)
    {
        assert(c.count(i) == 1);
        assert(c.find(i)->second == i + 1);
    }
}

int main(int, char**)
{
    std::unordered_multimap<int, int> m;
    test(m);
    std::unordered_multimap<int, int, std::hash<int>, std::equal_to<int>, min_allocator<std::pair<const int, int>>> m2;
    test(m2);

  return 0;

    return 0;
}
} // libcis_ns_containers_unord_unord_multimap_unord_multimap_modifiers_insert_node_type_hint_2ee1cdab (libcis)

