// AST-transferred from libc++ by tools/transfer.py (slug=containers_unord_unord_set_bucket_size).
// main -> test_containers_unord_unord_set_bucket_size; file-scope helpers isolated in anon namespace.
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
// class unordered_set

// size_type bucket_size(size_type n) const

#include <unordered_set>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"

void test_containers_unord_unord_set_bucket_size() {
  {
    typedef std::unordered_set<int> C;
    typedef int P;
    P a[] = {P(1), P(2), P(3), P(4), P(1), P(2)};
    const C c(std::begin(a), std::end(a));
    assert(c.bucket_count() >= 5);
    LIBCPP_ASSERT(c.bucket_size(0) == 0);
    LIBCPP_ASSERT(c.bucket_size(1) == 1);
    LIBCPP_ASSERT(c.bucket_size(2) == 1);
    LIBCPP_ASSERT(c.bucket_size(3) == 1);
    LIBCPP_ASSERT(c.bucket_size(4) == 1);
  }
#if TEST_STD_VER >= 11
  {
    typedef std::unordered_set<int, std::hash<int>, std::equal_to<int>, min_allocator<int>> C;
    typedef int P;
    P a[] = {P(1), P(2), P(3), P(4), P(1), P(2)};
    const C c(std::begin(a), std::end(a));
    assert(c.bucket_count() >= 5);
    LIBCPP_ASSERT(c.bucket_size(0) == 0);
    LIBCPP_ASSERT(c.bucket_size(1) == 1);
    LIBCPP_ASSERT(c.bucket_size(2) == 1);
    LIBCPP_ASSERT(c.bucket_size(3) == 1);
    LIBCPP_ASSERT(c.bucket_size(4) == 1);
  }
#endif

  return;
}
