// transferred+adapted from libc++ by tools/transfer.py (slug=containers_associative_multiset_get_allocator_f5a13bd4).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <set>

// class multiset

// allocator_type get_allocator() const

#include <set>
#include <cassert>

#include "test_allocator.h"
#include "test_macros.h"

namespace libcis_ns_containers_associative_multiset_get_allocator_f5a13bd4 { // libcis
int main(int, char**) {
    {
        std::allocator<int> alloc;
        const std::multiset<int> s(alloc);
        assert(s.get_allocator() == alloc);
    }
    {
        other_allocator<int> alloc(1);
        const std::multiset<int, std::less<int>, other_allocator<int> > s(alloc);
        assert(s.get_allocator() == alloc);
    }
    return 0;

    return 0;
}
} // libcis_ns_containers_associative_multiset_get_allocator_f5a13bd4 (libcis)

