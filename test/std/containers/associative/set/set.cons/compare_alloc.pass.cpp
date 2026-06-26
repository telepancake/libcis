// transferred+adapted from libc++ by tools/transfer.py (slug=containers_associative_set_set_cons_compare_alloc_7243219a).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <set>

// class set

// set(const value_compare& comp, const allocator_type& a);

#include <set>
#include <cassert>

#include "test_macros.h"
#include "../../../test_compare.h"
#include "test_allocator.h"

namespace libcis_ns_containers_associative_set_set_cons_compare_alloc_7243219a { // libcis
int main(int, char**)
{
    typedef test_less<int> C;
    typedef test_allocator<int> A;
    std::set<int, C, A> m(C(4), A(5));
    assert(m.empty());
    assert(m.begin() == m.end());
    assert(m.key_comp() == C(4));
    assert(m.get_allocator() == A(5));

  return 0;

    return 0;
}
} // libcis_ns_containers_associative_set_set_cons_compare_alloc_7243219a (libcis)

