// AST-transferred from libc++ by tools/transfer.py (slug=containers_unord_unord_map_unord_map_modifiers_insert_init).
// main -> test_containers_unord_unord_map_unord_map_modifiers_insert_init; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <unordered_map>

// template <class Key, class T, class Hash = hash<Key>, class Pred = equal_to<Key>,
//           class Alloc = allocator<pair<const Key, T>>>
// class unordered_map

// void insert(initializer_list<value_type> il);

#include <unordered_map>
#include <string>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "min_allocator.h"

void test_containers_unord_unord_map_unord_map_modifiers_insert_init() {
  {
    typedef std::unordered_map<int, std::string> C;
    typedef std::pair<int, std::string> P;
    C c;
    c.insert({
        P(1, "one"),
        P(2, "two"),
        P(3, "three"),
        P(4, "four"),
        P(1, "four"),
        P(2, "four"),
    });
    assert(c.size() == 4);
    assert(c.at(1) == "one");
    assert(c.at(2) == "two");
    assert(c.at(3) == "three");
    assert(c.at(4) == "four");
  }
  {
    typedef std::unordered_map<int,
                               std::string,
                               std::hash<int>,
                               std::equal_to<int>,
                               min_allocator<std::pair<const int, std::string>>>
        C;
    typedef std::pair<int, std::string> P;
    C c;
    c.insert({
        P(1, "one"),
        P(2, "two"),
        P(3, "three"),
        P(4, "four"),
        P(1, "four"),
        P(2, "four"),
    });
    assert(c.size() == 4);
    assert(c.at(1) == "one");
    assert(c.at(2) == "two");
    assert(c.at(3) == "three");
    assert(c.at(4) == "four");
  }

  return;
}
