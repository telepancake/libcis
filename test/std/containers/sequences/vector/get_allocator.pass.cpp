// transferred+adapted from libc++ by tools/transfer.py (slug=containers_sequences_vector_get_allocator_7b4e5072).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <vector>

// class vector

// allocator_type get_allocator() const

#include <vector>
#include <cassert>

#include "test_allocator.h"
#include "test_macros.h"

namespace libcis_ns_containers_sequences_vector_get_allocator_7b4e5072 { // libcis
TEST_CONSTEXPR_CXX20 bool test() {
    {
        std::allocator<int> alloc;
        const std::vector<int> v(alloc);
        assert(v.get_allocator() == alloc);
    }
    {
        other_allocator<int> alloc(1);
        const std::vector<int, other_allocator<int> > v(alloc);
        assert(v.get_allocator() == alloc);
    }

    return true;
}

int main(int, char**) {
    test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif

    return 0;

    return 0;
}
} // libcis_ns_containers_sequences_vector_get_allocator_7b4e5072 (libcis)

