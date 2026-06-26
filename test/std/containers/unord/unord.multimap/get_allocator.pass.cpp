// transferred+adapted from libc++ by tools/transfer.py (slug=containers_unord_unord_multimap_get_allocator_7ff3c758).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <unordered_map>

// class unordered_multimap

// allocator_type get_allocator() const

#include <unordered_map>
#include <cassert>
#include <string>

#include "test_allocator.h"
#include "test_macros.h"

namespace libcis_ns_containers_unord_unord_multimap_get_allocator_7ff3c758 { // libcis
int main(int, char**) {
    typedef std::pair<const int, std::string> ValueType;
    {
        std::allocator<ValueType> alloc;
        const std::unordered_multimap<int, std::string> m(alloc);
        assert(m.get_allocator() == alloc);
    }
    {
        other_allocator<ValueType> alloc(1);
        const std::unordered_multimap<int, std::string, std::hash<int>,
                                      std::equal_to<int>,
                                      other_allocator<ValueType> > m(alloc);
        assert(m.get_allocator() == alloc);
    }

    return 0;

    return 0;
}
} // libcis_ns_containers_unord_unord_multimap_get_allocator_7ff3c758 (libcis)

