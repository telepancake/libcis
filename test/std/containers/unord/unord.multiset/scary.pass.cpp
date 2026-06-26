// transferred+adapted from libc++ by tools/transfer.py (slug=containers_unord_unord_multiset_scary_ac2f24b6).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <unordered_set>

// class unordered_set class unordered_multiset

// Extension:  SCARY/N2913 iterator compatibility between unordered_set and unordered_multiset

#include <unordered_set>

#include "test_macros.h"

namespace libcis_ns_containers_unord_unord_multiset_scary_ac2f24b6 { // libcis
int main(int, char**)
{
    typedef std::unordered_set<int> M1;
    typedef std::unordered_multiset<int> M2;
    M2::iterator i;
    M1::iterator j = i;
    ((void)j);

  return 0;

    return 0;
}
} // libcis_ns_containers_unord_unord_multiset_scary_ac2f24b6 (libcis)

