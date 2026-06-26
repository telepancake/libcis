// transferred+adapted from libc++ by tools/transfer.py (slug=containers_unord_unord_map_empty_ae077e1f).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <unordered_map>

// class unordered_map

// bool empty() const noexcept;

#include <unordered_map>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"

namespace libcis_ns_containers_unord_unord_map_empty_ae077e1f { // libcis
int main(int, char**)
{
    {
    typedef std::unordered_map<int, double> M;
    M m;
    ASSERT_NOEXCEPT(m.empty());
    assert(m.empty());
    m.insert(M::value_type(1, 1.5));
    assert(!m.empty());
    m.clear();
    assert(m.empty());
    }
#if TEST_STD_VER >= 11
    {
    typedef std::unordered_map<int, double, std::hash<int>, std::equal_to<int>, min_allocator<std::pair<const int, double>>> M;
    M m;
    ASSERT_NOEXCEPT(m.empty());
    assert(m.empty());
    m.insert(M::value_type(1, 1.5));
    assert(!m.empty());
    m.clear();
    assert(m.empty());
    }
#endif

  return 0;

    return 0;
}
} // libcis_ns_containers_unord_unord_map_empty_ae077e1f (libcis)

