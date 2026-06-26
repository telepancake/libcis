// transferred+adapted from libc++ by tools/transfer.py (slug=containers_associative_set_set_cons_compare_4e6734c6).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <set>

// class set

// explicit set(const key_compare& comp) const;

#include <set>
#include <cassert>

#include "test_macros.h"
#include "../../../test_compare.h"

namespace libcis_ns_containers_associative_set_set_cons_compare_4e6734c6 { // libcis
int main(int, char**)
{
    typedef test_less<int> C;
    const std::set<int, C> m(C(3));
    assert(m.empty());
    assert(m.begin() == m.end());
    assert(m.key_comp() == C(3));
    assert(m.value_comp() == C(3));

  return 0;

    return 0;
}
} // libcis_ns_containers_associative_set_set_cons_compare_4e6734c6 (libcis)

