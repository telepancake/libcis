// transferred+adapted from libc++ by tools/transfer.py (slug=containers_sequences_list_list_capacity_resize_size_value_1682d417).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <list>

// void resize(size_type sz, const value_type& x);

#include <list>
#include <cassert>
#include "test_macros.h"
#include "DefaultOnly.h"
#include "min_allocator.h"

namespace libcis_ns_containers_sequences_list_list_capacity_resize_size_value_1682d417 { // libcis
int main(int, char**)
{
    {
        std::list<double> l(5, 2);
        l.resize(2, 3.5);
        assert(l.size() == 2);
        assert(std::distance(l.begin(), l.end()) == 2);
        assert(l == std::list<double>(2, 2));
    }
    {
        std::list<double> l(5, 2);
        l.resize(10, 3.5);
        assert(l.size() == 10);
        assert(std::distance(l.begin(), l.end()) == 10);
        assert(l.front() == 2);
        assert(l.back() == 3.5);
    }
#if TEST_STD_VER >= 11
    {
        std::list<double, min_allocator<double>> l(5, 2);
        l.resize(2, 3.5);
        assert(l.size() == 2);
        assert(std::distance(l.begin(), l.end()) == 2);
        assert((l == std::list<double, min_allocator<double>>(2, 2)));
    }
    {
        std::list<double, min_allocator<double>> l(5, 2);
        l.resize(10, 3.5);
        assert(l.size() == 10);
        assert(std::distance(l.begin(), l.end()) == 10);
        assert(l.front() == 2);
        assert(l.back() == 3.5);
    }
#endif

  return 0;

    return 0;
}
} // libcis_ns_containers_sequences_list_list_capacity_resize_size_value_1682d417 (libcis)

