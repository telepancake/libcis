// transferred+adapted from libc++ by tools/transfer.py (slug=containers_sequences_list_list_cons_op_equal_initializer_list_4b1e087e).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <list>

// list& operator=(initializer_list<value_type> il);

#include <list>
#include <cassert>
#include "test_macros.h"
#include "min_allocator.h"

namespace libcis_ns_containers_sequences_list_list_cons_op_equal_initializer_list_4b1e087e { // libcis
int main(int, char**)
{
    {
    std::list<int> d;
    d = {3, 4, 5, 6};
    assert(d.size() == 4);
    std::list<int>::iterator i = d.begin();
    assert(*i++ == 3);
    assert(*i++ == 4);
    assert(*i++ == 5);
    assert(*i++ == 6);
    }
    {
    std::list<int, min_allocator<int>> d;
    d = {3, 4, 5, 6};
    assert(d.size() == 4);
    std::list<int, min_allocator<int>>::iterator i = d.begin();
    assert(*i++ == 3);
    assert(*i++ == 4);
    assert(*i++ == 5);
    assert(*i++ == 6);
    }

  return 0;

    return 0;
}
} // libcis_ns_containers_sequences_list_list_cons_op_equal_initializer_list_4b1e087e (libcis)

