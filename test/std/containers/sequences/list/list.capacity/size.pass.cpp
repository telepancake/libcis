// transferred+adapted from libc++ by tools/transfer.py (slug=containers_sequences_list_list_capacity_size_6bf89232).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <list>

// class list

// size_type size() const noexcept;

#include <list>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"

namespace libcis_ns_containers_sequences_list_list_capacity_size_6bf89232 { // libcis
int main(int, char**)
{
    {
    typedef std::list<int> C;
    C c;
    ASSERT_NOEXCEPT(c.size());
    assert(c.size() == 0);
    c.push_back(C::value_type(2));
    assert(c.size() == 1);
    c.push_back(C::value_type(1));
    assert(c.size() == 2);
    c.push_back(C::value_type(3));
    assert(c.size() == 3);
    c.erase(c.begin());
    assert(c.size() == 2);
    c.erase(c.begin());
    assert(c.size() == 1);
    c.erase(c.begin());
    assert(c.size() == 0);
    }
#if TEST_STD_VER >= 11
    {
    typedef std::list<int, min_allocator<int>> C;
    C c;
    ASSERT_NOEXCEPT(c.size());
    assert(c.size() == 0);
    c.push_back(C::value_type(2));
    assert(c.size() == 1);
    c.push_back(C::value_type(1));
    assert(c.size() == 2);
    c.push_back(C::value_type(3));
    assert(c.size() == 3);
    c.erase(c.begin());
    assert(c.size() == 2);
    c.erase(c.begin());
    assert(c.size() == 1);
    c.erase(c.begin());
    assert(c.size() == 0);
    }
#endif

  return 0;

    return 0;
}
} // libcis_ns_containers_sequences_list_list_capacity_size_6bf89232 (libcis)

