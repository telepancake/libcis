// AST-transferred from libc++ by tools/transfer.py (slug=containers_unord_unord_map_get_allocator).
// main -> test_containers_unord_unord_map_get_allocator; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <unordered_map>

// class unordered_map

// allocator_type get_allocator() const

#include <unordered_map>
#include <cassert>
#include <string>

#include "test_allocator.h"
#include "test_macros.h"

void test_containers_unord_unord_map_get_allocator() {
  typedef std::pair<const int, std::string> ValueType;
  {
    std::allocator<ValueType> alloc;
    const std::unordered_map<int, std::string> m(alloc);
    assert(m.get_allocator() == alloc);
  }
  {
    other_allocator<ValueType> alloc(1);
    const std::unordered_map<int, std::string, std::hash<int>, std::equal_to<int>, other_allocator<ValueType> > m(
        alloc);
    assert(m.get_allocator() == alloc);
  }

  return;
}
