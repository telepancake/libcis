// transferred+adapted from libc++ by tools/transfer.py (slug=containers_sequences_list_list_modifiers_push_back_f3f61b51).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <list>

// void push_back(const value_type& x);

#include <list>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"

namespace libcis_ns_containers_sequences_list_list_modifiers_push_back_f3f61b51 { // libcis
int main(int, char**)
{
    {
    std::list<int> c;
    for (int i = 0; i < 5; ++i)
        c.push_back(i);
    int a[] = {0, 1, 2, 3, 4};
    assert(c == std::list<int>(a, a+5));
    }
#if TEST_STD_VER >= 11
    {
    std::list<int, min_allocator<int>> c;
    for (int i = 0; i < 5; ++i)
        c.push_back(i);
    int a[] = {0, 1, 2, 3, 4};
    assert((c == std::list<int, min_allocator<int>>(a, a+5)));
    }
#endif

  return 0;

    return 0;
}
} // libcis_ns_containers_sequences_list_list_modifiers_push_back_f3f61b51 (libcis)

